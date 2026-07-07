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

void ReaderDialog::OnBack(wxCommandEvent& event) {
  (void)event;
  if (history_.size() <= 1)
    return;
  history_.pop_back();
  const auto& prev = history_.back();
  history_.pop_back();  // Navigate will push it back
  Navigate(prev.surah, prev.begin_ayah, prev.end_ayah);
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

wxString ReaderDialog::BuildHtml(uint surah, uint begin_ayah, uint end_ayah) {
  wxString html =
      "<html><head><style>"
      "body { background:#282c34; color:#ffffff; "
      "font-family: 'Noto Naskh Arabic'; }"
      "table { width:100%; max-width:900px; margin:0 auto; }"
      "td { vertical-align:top; padding:10px; }"
      ".ar { direction:rtl; text-align:right; font-size:24px; width:50%; }"
      ".en { direction:ltr; text-align:left; font-size:16px; width:50%; }"
      "sup { color:#4ea1ff; cursor:pointer; }"
      ".note-row a { color:#4ea1ff !important; cursor:pointer; "
      "text-decoration:none; }"
      ".note-row a:hover { text-decoration:underline; }"
      "</style></head><body><table>";

  const auto& all_verses = quranite_.getVerse()[surah - 1];
  const auto& all_notes = quranite_.getNote();

  uint ayah = begin_ayah;
  for (uint i = begin_ayah - 1; i < end_ayah && i < all_verses.size(); i++) {
    const auto& ver = all_verses[i];

    wxString notes_json = "[";

    html += wxString::Format(
        "<tr data-ayah=\"%u\" data-notes='%s'>"
        "<td class=\"ar\">%s</td><td class=\"en\">%s</td></tr>",
        ayah, notes_json, wxString::FromUTF8(ver.arabic),
        wxString::FromUTF8(ver.english));

    const auto& ayah_notes = all_notes[surah - 1][ayah - 1];
    for (size_t n = 0; n < ayah_notes.size(); n++) {
      wxString note_str = wxString::FromUTF8(ayah_notes[n]);
      unsigned long a, b, c;
      if (std::sscanf(note_str, "%lu:%lu:%lu", &a, &b, &c) == 3) {
        if (a > 0 && a <= all_notes.size() && b > 0 &&
            b <= all_notes[a - 1].size() && c > 0 &&
            c <= all_notes[a - 1][b - 1].size()) {
          note_str = wxString::FromUTF8(all_notes[a - 1][b - 1][c - 1]);
        }
      }
      html += wxString::Format(
          "<tr class=\"note-row\"><td colspan=\"2\" "
          "style=\"color:#aaa; font-size:13px; padding:6px 10px; "
          "border-bottom:1px solid #444;\">%s</td></tr>",
          note_str);
    }

    ayah++;
  }

  html +=
      "</table><script>"
      "document.querySelectorAll('sup').forEach(e => {"
      "  e.addEventListener('click', () => {"
      "    const row = e.closest('tr');"
      "    const ayah = row.dataset.ayah;"
      "    const noteIndex = parseInt(e.textContent) - 1;"
      "    const key = ayah + '_' + noteIndex;"
      "    const openNotes = JSON.parse(row.dataset.openNotes || '{}');"
      "    const isOpen = openNotes[key] !== undefined;"
      "    if (isOpen) {"
      "      delete openNotes[key];"
      "    } else {"
      "      const notes = JSON.parse(row.dataset.notes);"
      "      openNotes[key] = { index: noteIndex, ayah: ayah, text: "
      "notes[noteIndex] };"
      "    }"
      "    row.dataset.openNotes = JSON.stringify(openNotes);"
      "    while (row.nextElementSibling?.classList.contains('note-row')) {"
      "      row.nextElementSibling.remove();"
      "    }"
      "    const notesHtml = Object.values(openNotes)"
      "      .sort((a, b) => a.index - b.index)"
      "      .map(n => '<tr class=\"note-row\"><td colspan=\"2\" "
      "style=\"color:#aaa; font-size:13px; padding:6px 10px; "
      "border-bottom:1px solid #444;\">' + n.text + '</td></tr>')"
      "      .join('');"
      "    row.insertAdjacentHTML('afterend', notesHtml);"
      "  });"
      "});"
      "document.addEventListener('click', e => {"
      "  const a = e.target.closest('a');"
      "  if (a && a.closest('.note-row')) {"
      "    e.preventDefault();"
      "    window.manzil.postMessage(JSON.stringify({"
      "      ref: a.textContent.trim()"
      "    }));"
      "  }"
      "});"
      "</script></body></html>";

  return html;
}
