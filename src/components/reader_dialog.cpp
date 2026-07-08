#include "reader_dialog.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include "../types.hpp"

using json = nlohmann::json;

ReaderDialog::ReaderDialog(wxWindow* parent, Quranite& quranite,
                           manzil::nav_entry entry)
    : wxDialog(parent, wxID_ANY, "Note", wxDefaultPosition, wxDefaultSize,
               wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
      quranite_(quranite) {

  back_btn_ = new wxButton(this, wxID_ANY, "<< Back");
  back_btn_->Bind(wxEVT_BUTTON, &ReaderDialog::OnBack, this);
  back_btn_->Hide();

  webview_ = wxWebView::New(this, wxID_ANY);
  webview_->AddScriptMessageHandler("manzil");
  webview_->Bind(wxEVT_WEBVIEW_SCRIPT_MESSAGE_RECEIVED,
                 &ReaderDialog::OnVerseRef, this);

  constexpr int back_btn_border = 5;

  auto* sizer = new wxBoxSizer(wxVERTICAL);
  sizer->Add(back_btn_, 0, wxALL, back_btn_border);
  sizer->Add(webview_, 1, wxEXPAND);
  SetSizer(sizer);

  Navigate(entry);
}

void ReaderDialog::Navigate(manzil::nav_entry entry) {
  history_.push_back(entry);
  back_btn_->Show(history_.size() > 1);
  Layout();
  webview_->SetPage(BuildHtml(entry), "");
}

//TODO: OnBack should not pop history_
//      it should just show the previous dialog
//      like how "back" should be
//      so there might be a need for next btn
void ReaderDialog::OnBack(wxCommandEvent& event) {
  (void)event;
  if (history_.size() <= 1) {
    return;
  }
  history_.pop_back();
  const auto& prev = history_.back();
  history_.pop_back();
  Navigate(prev);
}

void ReaderDialog::OnVerseRef(wxWebViewEvent& event) {
  json webview_json;
  try {
    webview_json = json::parse(event.GetString().ToStdString());
  } catch (const std::exception& e) {
    std::cerr
        << "<< Manzil: ReaderDialog::OnVerseRef webview json parse error: "
        << e.what() << "\n";
  }
  if (!webview_json.contains("ref")) {
    return;
  }

  manzil::nav_entry entry;

  std::string webview_str = webview_json["ref"].get<std::string>();
  if (std::sscanf(webview_str.c_str(), "%u:%u-%u", &entry.surah,
                  &entry.begin_ayah, &entry.end_ayah) == 3) {
  }

  else if (std::sscanf(webview_str.c_str(), "%u:%u", &entry.surah,
                       &entry.begin_ayah) == 2) {
    entry.end_ayah = entry.begin_ayah;
  }

  else {
    std::cerr
        << "<< Manzil: ReaderDialog::OnVerseRef webview string parse error \n";
    return;
  }

  Navigate(entry);
}

wxString ReaderDialog::BuildHtml(manzil::nav_entry entry) {
  wxString html =
      "<html><head><style>"
      "body { background:#282c34; color:#ffffff; "
      "font-family: 'Noto Naskh Arabic'; }"
      "table { width:100%; max-width:900px; margin:0 auto; }"
      "td { vertical-align:top; padding:10px; }"
      ".ar { direction:rtl; text-align:right; font-size:24px; width:50%; }"
      ".en { direction:ltr; text-align:left; font-size:16px; width:50%; }"
      "sup { color:#4ea1ff; cursor:pointer; }"
      "a { color:#4ea1ff; cursor:pointer; }"
      ".note-row td { color:#aaa; font-size:13px; padding:6px 10px; "
      "border-bottom:1px solid #444; }"
      ".note-row.hidden { display: note-row; }"
      "</style></head><body><table>";

  const auto& surah_verses = quranite_.GetVerse()[entry.surah - 1];
  const auto& all_notes = quranite_.GetNote();

  for (unsigned int ayah_idx = entry.begin_ayah - 1;
       ayah_idx < entry.end_ayah && ayah_idx < surah_verses.size();
       ayah_idx++) {
    const auto& verse = surah_verses[ayah_idx];

    wxString notes_json = "[";

    html += wxString::Format(
        "<tr data-ayah=\"%u\" data-notes='%s'>"
        "<td class=\"ar\">%s</td><td class=\"en\">%s</td></tr>",
        ayah_idx, notes_json, wxString::FromUTF8(verse.arabic),
        wxString::FromUTF8(verse.english));

    const auto& ayah_notes = all_notes[entry.surah - 1][ayah_idx];
    for (const auto& ayah_note : ayah_notes) {
      wxString note_str = wxString::FromUTF8(ayah_note);

      unsigned int surah_n;
      unsigned int ayah_n;
      unsigned int note_n;
      if (std::sscanf(note_str, "%u:%u:%u", &surah_n, &ayah_n, &note_n) == 3) {

        if (surah_n > 0 &&                                        //
            surah_n <= all_notes.size() &&                        //
            ayah_n > 0 &&                                         //
            ayah_n <= all_notes[surah_n - 1].size() &&            //
            note_n > 0 &&                                         //
            note_n <= all_notes[surah_n - 1][ayah_n - 1].size())  //
        {
          note_str = wxString::FromUTF8(
              all_notes[surah_n - 1][ayah_n - 1][note_n - 1]);
        } else {
          // Reference parsed but indices out of range, malformed data.
          std::cerr << "<< Manzil: malformed note reference \"" << ayah_note
                    << "\", showing raw text\n";
          note_str =
              wxString::Format("[malformed note reference: %s]", note_str);
        }

      } else {
        // sscanf failed to match all 3 fields, this is plain note text,
        // not a cross-reference. note_str already holds the correct value.
      }

      html += wxString::Format(
          "<tr class=\"note-row\"><td colspan=\"2\" "
          "style=\"color:#aaa; font-size:13px; padding:6px 10px; "
          "border-bottom:1px solid #444;\">%s</td></tr>",
          note_str);
    }
  }

  html +=
      "</table><script>"
      ""
      "// 1. INITIALIZE: Parse JSON and insert all note rows into the DOM "
      "immediately\n"
      ""
      "document.querySelectorAll('tr[data-notes]').forEach(row => {\n"
      "  const notes = JSON.parse(row.dataset.notes || '[]');\n"
      "  const ayah = row.dataset.ayah;\n"
      "  \n"
      "  // Map each note text into its own table row markup\n"
      "  const notesHtml = notes.map((text, index) => `\n"
      "    <tr class='note-row hidden' data-ayah='${ayah}' "
      "data-index='${index}'>\n"
      "      <td colspan='2'>${text}</td>\n"
      "    </tr>\n"
      "  `).join('');\n"
      "  \n"
      "  // Inject the notes directly underneath this verse row\n"
      "  row.insertAdjacentHTML('afterend', notesHtml);\n"
      "});\n"
      "\n"
      ""
      "// 2. TOGGLE: Show or hide individual notes when clicking their 'sup' "
      "tags\n"
      ""
      "document.querySelectorAll('sup').forEach(sup => {\n"
      "  sup.addEventListener('click', () => {\n"
      "    const row = sup.closest('tr');\n"
      "    const ayah = row.dataset.ayah;\n"
      "    const index = parseInt(sup.textContent) - 1; // '1' becomes index "
      "0\n"
      "    \n"
      "    // Target the specific note row using data attributes\n"
      "    const noteRow = "
      "document.querySelector(`tr.note-row[data-ayah='${ayah}'][data-index='${"
      "index}']`);\n"
      "    if (noteRow) {\n"
      "      noteRow.classList.toggle('hidden');\n"
      "    }\n"
      "  });\n"
      "});\n"
      "\n"
      ""
      "// 3. CROSS-REFERENCES: Intercept clicks on links inside notes\n"
      ""
      "document.addEventListener('click', e => {\n"
      "  const a = e.target.closest('a');\n"
      "  if (a && a.closest('.note-row')) {\n"
      "    e.preventDefault();\n"
      "    window.manzil.postMessage(JSON.stringify({ ref: "
      "a.textContent.trim() }));\n"
      "  }\n"
      "});\n"
      "</script></body></html>";

  return html;
}
