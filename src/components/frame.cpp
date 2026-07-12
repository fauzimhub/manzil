#include "frame.hpp"
#include <iostream>
#include "../events.hpp"
#include "../manzil.hpp"
#include "about_dialog.hpp"
#include "quranite.hpp"
#include "reader.hpp"
#include "surah_card.hpp"

using std::cerr;
using std::exception;
using std::filesystem::read_symlink;

Frame::Frame(const wxString& title, int min_width, int min_height)
    : wxFrame(nullptr, wxID_ANY, title),
      quranite_([]() {
        path exe_dir = manzil::GetExecutableDir();
        manzil::quranite_data_paths paths;
        paths.surah = (exe_dir / "assets" / "chapters-data.json").string();
        paths.ar = (exe_dir / "assets" / "verses_ar_original.json").string();
        paths.en =
            (exe_dir / "assets" / "verses_en_sam-gerrans_with-notes.json")
                .string();
        paths.notes =
            (exe_dir / "assets" / "notes_en_sam-gerrans.json").string();
        return Quranite(paths);
      }())

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
  constexpr int panel_ystep = 10;

  surah_list_ = new wxScrolledWindow(this);

  grid_ = new wxGridSizer(grid_cols, grid_hgap, grid_vgap);
  for (const auto& sur : quranite_.GetSurah()) {
    auto* card = new SurahCard(surah_list_, wxString::Format("%d", sur.number),
                               wxString::FromUTF8(sur.name_arabic),
                               sur.name_transliteration,
                               wxString::FromUTF8(sur.name_translation),
                               wxString::Format("%d", sur.verses_count));

    grid_->Add(card, 0, wxEXPAND | wxALL, grid_padding);
  }

  surah_list_->SetSizer(grid_);
  surah_list_->FitInside();
  surah_list_->SetScrollRate(0, panel_ystep);

  auto* sizer = new wxBoxSizer(wxVERTICAL);
  sizer->Add(surah_list_, 1, wxEXPAND);
  SetSizer(sizer);

  surah_list_->Show();

  Bind(wxEVT_MENU, &Frame::OnAbout, this, wxID_ABOUT);
  Bind(wxEVT_MENU, &Frame::OnQuit, this, wxID_EXIT);
  Bind(EVT_SURAH_SELECTED, &Frame::OnSurahSelected, this);
  Bind(wxEVT_CHAR_HOOK, &Frame::OnKeyDown, this);
  Bind(wxEVT_SIZE, &Frame::OnSize, this);
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
  surah_number_ = static_cast<unsigned int>(event.GetInt());
  const auto surah_data = quranite_.GetSurah()[surah_number_ - 1];

  if (reader_ == nullptr) {
    header_card_ = HeaderCard::MakeReaderMode(
        this, wxString::Format("%d", surah_number_),
        wxString::FromUTF8(surah_data.name_arabic),
        surah_data.name_transliteration,
        wxString::FromUTF8(surah_data.name_translation));

    reader_ = new Reader(this, quranite_, surah_number_);
    GetSizer()->Add(header_card_, 1, wxEXPAND);
    GetSizer()->Add(reader_, 1, wxEXPAND);
  } else {
    reader_->LoadSurah(surah_number_);
    header_card_->SetData(wxString::Format("%d", surah_number_),
                          wxString::FromUTF8(surah_data.name_arabic),
                          surah_data.name_transliteration,
                          wxString::FromUTF8(surah_data.name_translation));
  }

  surah_list_->Hide();
  header_card_->Show();
  reader_->Show();
  Layout();
  reader_->SetFocus();
}

void Frame::OnKeyDown(wxKeyEvent& event) {
  if (event.GetKeyCode() == WXK_ESCAPE && reader_ != nullptr &&
      reader_->IsShown()) {
    header_card_->Hide();
    reader_->Hide();
    surah_list_->Show();
    Layout();
  } else {
    event.Skip();
  }
}

void Frame::OnSize(wxSizeEvent& event) {
  event.Skip();
  int cols = static_cast<int>(
      std::trunc(GetClientSize().GetWidth() / k_grid_col_divisor));
  if (cols != grid_->GetCols()) {
    grid_->SetCols(cols);
    surah_list_->FitInside();
    surah_list_->Layout();
  }
}
