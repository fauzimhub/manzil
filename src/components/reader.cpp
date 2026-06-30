#include "reader.hpp"
#include "../types.hpp"
#include "quranite.hpp"

Reader::Reader(wxWindow* parent, Quranite& quranite, uint surah_number)
    : wxPanel(parent, wxID_ANY), quranite_(quranite) {
  webview_ = wxWebView::New(this, wxID_ANY);
  webview_->SetPage(BuildHtml(quranite, surah_number), "");

  auto* sizer = new wxBoxSizer(wxVERTICAL);
  sizer->Add(webview_, 1, wxEXPAND);
  SetSizer(sizer);
}

wxString Reader::BuildHtml(Quranite& quranite, uint surah_number) {
  wxString html =
      "<html><head><style>"
      "body { background:#282c34; color:#ffffff;  "
      "font-family: 'Noto Naskh Arabic'; }"
      "table { width:100%;}"
      "td { vertical-align:top; padding:10px;}"
      ".ar { direction:rtl; text-align:right; font-size:24px; width:50%; }"
      ".en { direction:ltr; text-align:left; font-size:16px; width:50%; }"
      "</style></head><body><table>";

  manzil::verse_list verse = quranite.getVerse();

  for (const auto& ver : verse[surah_number - 1]) {
    html += wxString::Format(
        "<tr><td class=\"ar\">%s</td><td class=\"en\">%s</td></tr>",
        wxString::FromUTF8(ver.arabic), wxString::FromUTF8(ver.english));
  }

  html += "</table></body></html>";
  return html;
}

void Reader::LoadSurah(uint surah_number) {
  webview_->SetPage(BuildHtml(quranite_, surah_number), "");
}
