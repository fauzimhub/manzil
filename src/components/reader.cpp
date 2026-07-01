#include "reader.hpp"
#include <nlohmann/json.hpp>
#include "../events.hpp"
#include "../types.hpp"
#include "quranite.hpp"

using json = nlohmann::json;

Reader::Reader(wxWindow* parent, Quranite& quranite, uint surah_number)
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

wxString Reader::BuildHtml(const Quranite& quranite, uint surah_number) {
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
      "</style></head><body><table>";

  manzil::verse_list verse = quranite.getVerse();
  manzil::note_list notes = quranite.getNote();

  uint ayah = 1;
  for (const auto& ver : verse[surah_number - 1]) {
    wxString notes_json = "[";
    for (size_t i = 0; i < notes[surah_number - 1][ayah - 1].size(); i++) {
      if (i > 0)
        notes_json += ",";

      wxString note_str =
          wxString::FromUTF8(notes[surah_number - 1][ayah - 1][i]);

      unsigned long a, b, c;
      if (std::sscanf(note_str, "%lu:%lu:%lu", &a, &b, &c) == 3) {
        if (a > 0 && a <= notes.size() && b > 0 && b <= notes[a - 1].size() &&
            c > 0 && c <= notes[a - 1][b - 1].size()) {
          note_str = wxString::FromUTF8(notes[a - 1][b - 1][c - 1]);
        }
      }

      notes_json += "\"" + note_str + "\"";
    }
    notes_json += "]";

    html += wxString::Format(
        "<tr data-ayah=\"%u\" data-notes='%s'>"
        "<td class=\"ar\">%s</td><td class=\"en\">%s</td></tr>",
        ayah, notes_json, wxString::FromUTF8(ver.arabic),
        wxString::FromUTF8(ver.english));
    ayah++;
  }

  html +=
      "</table>"
      "<script>"
      "document.querySelectorAll('sup').forEach(e => {"
      "  e.addEventListener('click', () => {"
      "    const row = e.closest('tr');"
      "    const ayah = row.dataset.ayah;"
      "    const noteIndex = parseInt(e.textContent) - 1;"
      "    const key = ayah + '_' + noteIndex;"
      ""
      "    const openNotes = JSON.parse(row.dataset.openNotes || '{}');"
      "    const isOpen = openNotes[key] !== undefined;"
      ""
      "    if (isOpen) {"
      "      delete openNotes[key];"
      "    } else {"
      "      const notes = JSON.parse(row.dataset.notes);"
      "      openNotes[key] = { index: noteIndex, ayah: ayah, text: "
      "notes[noteIndex] };"
      "    }"
      "    row.dataset.openNotes = JSON.stringify(openNotes);"
      ""
      "    window.manzil.postMessage(JSON.stringify({"
      "      ayah: ayah,"
      "      note: e.textContent,"
      "      action: isOpen ? 'close' : 'open'"
      "    }));"
      ""
      "    while (row.nextElementSibling?.classList.contains('note-row')) {"
      "      row.nextElementSibling.remove();"
      "    }"
      ""
      "    const notesHtml = Object.values(openNotes)"
      "      .sort((a, b) => a.index - b.index)"
      "      .map(n => '<tr class=\"note-row\"><td colspan=\"2\" "
      "style=\"color:#aaa; font-size:13px; padding:6px 10px; border-bottom:1px "
      "solid #444;\">' + n.text + '</td></tr>')"
      "      .join('');"
      ""
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

void Reader::OnNoteClicked(wxWebViewEvent& event) {
  try {
    auto parsed_payload = json::parse(event.GetString().ToStdString());
    uint ayah = static_cast<uint>(
        std::stoul(parsed_payload["ayah"].get<std::string>()));
    uint note = static_cast<uint>(
        std::stoul(parsed_payload["note"].get<std::string>()));
    uint surah = surah_number_;

    NoteClickedEvent note_event(EVT_NOTE_CLICKED, GetId(), surah, ayah, note);
    wxPostEvent(GetParent(), note_event);

  } catch (const std::exception& e) {
    std::cerr << "<< Manzil : Failed to parse note clicked payload, "
              << e.what() << "\n";
  }
}

void Reader::LoadSurah(uint surah_number) {
  surah_number_ = surah_number;
  webview_->SetPage(BuildHtml(quranite_, surah_number), "");
}
