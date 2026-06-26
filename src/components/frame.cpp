#include "frame.hpp"
#include "about_dialog.hpp"

Frame::Frame(const wxString& title, int min_width, int min_height)
    : wxFrame(nullptr, wxID_ANY, title) {

  SetMinSize(wxSize(min_width, min_height));

  auto* file_menu = new wxMenu;
  file_menu->Append(wxID_EXIT, "E&xit\tAlt-X", "Quit this program");

  auto* help_menu = new wxMenu;
  help_menu->Append(wxID_ABOUT, "&About...\tF1", "Show about dialog");

  auto* menu_bar = new wxMenuBar();
  menu_bar->Append(file_menu, "&File");
  menu_bar->Append(help_menu, "&Help");
  SetMenuBar(menu_bar);

  Bind(wxEVT_MENU, &Frame::OnAbout, this, wxID_ABOUT);
  Bind(wxEVT_MENU, &Frame::OnQuit, this, wxID_EXIT);
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
