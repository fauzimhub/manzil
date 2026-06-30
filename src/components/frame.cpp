#include "frame.hpp"
#include <iostream>
#include "../events.hpp"
#include "about_dialog.hpp"
#include "quranite.hpp"
#include "surah_card.hpp"

using std::cout;

Frame::Frame(const wxString& title, int min_width, int min_height)
    : wxFrame(nullptr, wxID_ANY, title),
      quranite_("assets/chapters-data.json", "assets/verses_ar_original.json",
                "assets/verses_en_sam-gerrans_with-notes.json")

{

  SetMinSize(wxSize(min_width, min_height));

  auto* file_menu = new wxMenu;
  file_menu->Append(wxID_EXIT, "E&xit\tAlt-X", "Quit this program");

  auto* help_menu = new wxMenu;
  help_menu->Append(wxID_ABOUT, "&About...\tF1", "Show about dialog");

  auto* menu_bar = new wxMenuBar();
  menu_bar->Append(file_menu, "&File");
  menu_bar->Append(help_menu, "&Help");
  SetMenuBar(menu_bar);

  constexpr int grid_hgap = 4;
  constexpr int grid_vgap = 4;
  constexpr int grid_cols = 1;
  constexpr int grid_padding = 10;

  surah_list_ = new wxScrolledWindow(this);
  reader_ = new wxPanel(this);
  auto* grid = new wxGridSizer(grid_cols, grid_hgap, grid_vgap);

  std::string surah_path = "assets/chapters-data.json";
  std::string verses_ar = "assets/verses_ar_original.json";
  std::string verses_en = "assets/verses_en_sam-gerrans_with-notes.json";

  Quranite quranite(surah_path, verses_ar, verses_en);

  for (const auto& sur : quranite.getSurah()) {
  for (const auto& sur : quranite_.getSurah()) {
    auto* card = new SurahCard(surah_list_, wxString::Format("%d", sur.number),
                               wxString::FromUTF8(sur.name_arabic),
                               sur.name_transliteration, sur.name_translation,
                               wxString::Format("%d", sur.verses_count));

    cout << "Surah (Number: " << sur.number
         << ", Name Arabic: " << sur.name_arabic
         << ", Name Translation: " << sur.name_translation
         << ", Name Transliteration: " << sur.name_transliteration
         << ", Verses Count: " << sur.verses_count << ")" << "\n";

    grid->Add(card, 0, wxEXPAND | wxALL, grid_padding);
  }

  constexpr int panel_ystep = 10;
  surah_list_->SetSizer(grid);
  surah_list_->FitInside();
  surah_list_->SetScrollRate(0, panel_ystep);

  auto* sizer = new wxBoxSizer(wxVERTICAL);
  sizer->Add(surah_list_, 1, wxEXPAND);
  sizer->Add(reader_, 1, wxEXPAND);
  SetSizer(sizer);

  surah_list_->Show();
  reader_->Hide();

  Bind(wxEVT_MENU, &Frame::OnAbout, this, wxID_ABOUT);
  Bind(wxEVT_MENU, &Frame::OnQuit, this, wxID_EXIT);
  Bind(EVT_SURAH_SELECTED, &Frame::OnSurahSelected, this);
}

void Frame::OnAbout(wxCommandEvent& event) {
  (void)event;
  AboutDialog dialog(this);
  dialog.ShowModal();
}

void Frame::OnQuit(wxCommandEvent& event) {
  (void)event;
  Close();
}

void Frame::OnSurahSelected(wxCommandEvent& event) {
  (void)event;
  surah_list_->Hide();
  reader_->Show();
  Layout();
}
