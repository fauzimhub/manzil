#include "reader.hpp"
#include <nlohmann/json.hpp>
#include "../types.hpp"
#include "quranite.hpp"

using json = nlohmann::json;

Reader::Reader(wxWindow* parent, Quranite& quranite, unsigned int surah_number)
    : wxPanel(parent, wxID_ANY),
      quranite_(quranite),
      surah_number_(surah_number) {
  webview_ = wxWebView::New(this, wxID_ANY);

  webview_->AddScriptMessageHandler("manzil");
  webview_->Bind(wxEVT_WEBVIEW_SCRIPT_MESSAGE_RECEIVED, &Reader::OnNoteClicked,
                 this);

  webview_->SetPage(BuildHtml(quranite, surah_number), "");

  auto* sizer = new wxBoxSizer(wxVERTICAL);
  sizer->Add(webview_, 1, wxEXPAND);
  SetSizer(sizer);
}

wxString Reader::BuildHtml(const Quranite& quranite,
                           unsigned int surah_number) {
  wxString html =
      "<html><head><style>"
      "body { background:#282c34; color:#ffffff; "
      "font-family: 'Noto Naskh Arabic'; }"
      "table { width:100%; max-width:900px; margin:0 auto; }"
      "td { vertical-align:top; padding:10px;}"
      ".ar { direction:rtl; text-align:right; font-size:24px; width:50%; }"
      ".en { direction:ltr; text-align:left; font-size:16px; width:50%; }"
      "sup { color:#4ea1ff; cursor:pointer; }"
      "a { color:#4ea1ff; cursor:pointer; }"
      ".note-row td { color:#aaa; font-size:13px; padding:6px 10px; "
      "border-bottom:1px solid #444; }"
      ".note-row.hidden { display: none; }"
      "</style></head><body><table>";

  const auto& surah_verses = quranite.GetVerse()[surah_number - 1];
  const auto& all_notes = quranite.GetNote();

  unsigned int ayah = 1;
  for (const auto& verse : surah_verses) {

    json notes_array = json::array();

    const auto& ayah_notes = all_notes[surah_number - 1][ayah - 1];
    for (const auto& ayah_note : ayah_notes) {

      // 1. Keep it as std::string to avoid
      //    wxString encoding issues during JSON building
      std::string note_str = ayah_note;

      unsigned int surah_n;
      unsigned int ayah_n;
      unsigned int note_n;
      // NOLINTNEXTLINE(bugprone-unchecked-string-to-number-conversion)
      if (std::sscanf(note_str.c_str(), "%u:%u:%u", &surah_n, &ayah_n,
                      &note_n) == 3) {

        if (surah_n > 0 &&                                        //
            surah_n <= all_notes.size() &&                        //
            ayah_n > 0 &&                                         //
            ayah_n <= all_notes[surah_n - 1].size() &&            //
            note_n > 0 &&                                         //
            note_n <= all_notes[surah_n - 1][ayah_n - 1].size())  //
        {
          note_str = all_notes[surah_n - 1][ayah_n - 1][note_n - 1];
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

      // 2. Push the raw string.
      //    nlohmann::json automatically escapes \n, ", \, etc.
      notes_array.push_back(note_str);
    }

    // 3. dump() creates perfectly valid JSON. Convert to wxString safely.
    //    (By default, dump() also escapes non-ASCII characters to \uXXXX,
    //    ensuring wxString::FromUTF8 never fails even in ANSI builds).
    wxString notes_json = wxString::FromUTF8(notes_array.dump());

    // Optional but recommended:
    // Escape single quotes so they don't break your HTML attribute
    notes_json.Replace("'", "&#39;");

    html += wxString::Format(
        "<tr data-ayah=\"%u\" data-notes='%s'>"
        "<td class=\"ar\">%s</td><td class=\"en\">%s</td></tr>",
        ayah, notes_json, wxString::FromUTF8(verse.arabic),
        wxString::FromUTF8(verse.english));
    ayah++;
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

void Reader::OnNoteClicked(wxWebViewEvent& event) {
  json webview_json;

  try {
    webview_json = json::parse(event.GetString().ToStdString());
  } catch (const std::exception& e) {
    std::cerr << "<< Manzil : Failed to parse note clicked payload, "
              << e.what() << "\n";
  }

  if (!webview_json.contains("ref")) {
    return;
  }

  manzil::nav_entry entry;

  std::string webview_str = webview_json["ref"].get<std::string>();
  // NOLINTNEXTLINE(bugprone-unchecked-string-to-number-conversion)
  if (std::sscanf(webview_str.c_str(), "%u:%u-%u", &entry.surah,
                  &entry.begin_ayah, &entry.end_ayah) == 3) {
  }
  // NOLINTNEXTLINE(bugprone-unchecked-string-to-number-conversion)
  else if (std::sscanf(webview_str.c_str(), "%u:%u", &entry.surah,
                       &entry.begin_ayah) == 2) {
    entry.end_ayah = entry.begin_ayah;
  }

  else {
    std::cerr
        << "<< Manzil: ReaderDialog::OnVerseRef webview string parse error \n";
    return;
  }

  if (dialog_ == nullptr) {
    dialog_ = new ReaderDialog(this, quranite_, entry);
    dialog_->Bind(wxEVT_CLOSE_WINDOW, [this](wxCloseEvent& event) {
      dialog_->Destroy();
      dialog_ = nullptr;
      event.Skip();
    });
    dialog_->Show();
  } else {
    dialog_->Navigate(entry);
    dialog_->Raise();
  }
}

void Reader::LoadSurah(unsigned int surah_number) {
  surah_number_ = surah_number;
  webview_->SetPage(BuildHtml(quranite_, surah_number), "");
}
