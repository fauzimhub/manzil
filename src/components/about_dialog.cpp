#include <wx/hyperlink.h>
#include <wx/statline.h>
#include <wx/wx.h>

#include "about_dialog.hpp"

AboutDialog::AboutDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "About", wxDefaultPosition, wxDefaultSize,
               wxDEFAULT_DIALOG_STYLE) {

  constexpr int cols = 2;
  constexpr int grid_hgap = 8;
  constexpr int grid_vgap = 6;
  constexpr int margin = 16;

  auto* outer = new wxBoxSizer(wxVERTICAL);

  auto* grid = new wxFlexGridSizer(cols, wxSize(grid_hgap, grid_vgap));

  grid->Add(new wxStaticText(this, wxID_ANY, "Version:"));
  grid->Add(new wxStaticText(this, wxID_ANY,
                             wxString::Format("Manzil %s", APP_VERSION)));
  grid->Add(new wxStaticText(this, wxID_ANY, "License:"));
  grid->Add(new wxStaticText(this, wxID_ANY, "MIT"));
  grid->Add(new wxStaticText(this, wxID_ANY, "Source:"));
  grid->Add(new wxHyperlinkCtrl(this, wxID_ANY, "codeberg.org/fauzim/manzil",
                                "https://codeberg.org/fauzim/manzil"));

  auto* quranite_grid = new wxFlexGridSizer(cols, wxSize(grid_hgap, grid_vgap));
  auto* quranite_col = new wxBoxSizer(wxVERTICAL);

  quranite_col->Add(new wxStaticText(
      this, wxID_ANY, "Independent English translation by Sam Gerrans."));
  quranite_col->Add(new wxHyperlinkCtrl(this, wxID_ANY, "reader.quranite.com",
                                        "https://reader.quranite.com/"));

  quranite_grid->Add(new wxStaticText(this, wxID_ANY, "Translation:"), 0,
                     wxALIGN_TOP);
  quranite_grid->Add(quranite_col);

  outer->Add(grid, 0, wxEXPAND | wxALL, margin);
  outer->Add(new wxStaticLine(this), 0, wxEXPAND | wxLEFT | wxRIGHT, margin);
  outer->Add(quranite_grid, 0, wxEXPAND | wxALL, margin);
  outer->Add(CreateButtonSizer(wxOK), 0, wxEXPAND | wxBOTTOM | wxRIGHT, margin);

  SetSizerAndFit(outer);
};
