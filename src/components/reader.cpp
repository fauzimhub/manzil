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
      "table { width:100%;}"
      "td { vertical-align:top; padding:10px;}"
      ".ar { direction:rtl; text-align:right; font-size:24px; width:50%; }"
      ".en { direction:ltr; text-align:left; font-size:16px; width:50%; }"
      "sup { color:#4ea1ff; cursor:pointer; }"
      "</style></head><body><table>";

  manzil::verse_list verse = quranite.getVerse();
  uint ayah = 1;
  for (const auto& ver : verse[surah_number - 1]) {
    html += wxString::Format(
        "<tr data-ayah=\"%u\">"
        "<td class=\"ar\">%s</td><td class=\"en\">%s</td></tr>",
        ayah, wxString::FromUTF8(ver.arabic), wxString::FromUTF8(ver.english));
    ayah++;
  }

  html +=
      "</table><script>"
      "document.querySelectorAll('sup').forEach((e) => {"
      "  e.addEventListener('click', () => {"
      "    var ayah = e.closest('tr').dataset.ayah;"
      "    window.manzil.postMessage(JSON.stringify({"
      "      ayah: ayah, note: e.textContent"
      "    }));"
      "  });"
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
