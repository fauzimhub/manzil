#include <wx/wx.h>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "manzil.hpp"

using json = nlohmann::json;
using ifstream = std::ifstream;
using string = std::string;
using std::cout;

bool manzil::App::OnInit() {
  json surah{};
  string title = "no title";
  string surah_path = "assets/surah.json";

  ifstream file{surah_path};
  if (file.is_open()) {
    try {

      surah = json::parse(file);
      title = surah[0]["name_translation"].get<string>();

    } catch (std::exception& e) {
      std::cerr << "<< Manzil: encountered an error while trying to parse \""
                << surah_path << "\"" << " as json" << ": "
                << "\n"
                << e.what() << "\n";
    }
  } else {
    std::cerr << "<< Manzil: could not open \"" << surah_path << "\"\n";
  }

  cout << surah << "\n";

  auto* frame = new manzil::Frame(title);

  frame->Show();

  return true;
}

BEGIN_EVENT_TABLE(manzil::Frame, wxFrame)
EVT_MENU(wxID_ABOUT, manzil::Frame::OnAbout)
EVT_MENU(wxID_EXIT, manzil::Frame::OnQuit)
END_EVENT_TABLE()

manzil::Frame::Frame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title) {

  wxMenu* file_menu = new wxMenu;
  wxMenu* help_menu = new wxMenu;

  help_menu->Append(wxID_ABOUT, wxT("&About...\tF1"), wxT("Show about dialog"));
  file_menu->Append(wxID_EXIT, wxT("E&xit\tAlt-X"), wxT("Quit this program"));
  wxMenuBar* menu_bar = new wxMenuBar();
  menu_bar->Append(file_menu, wxT("&File"));
  menu_bar->Append(help_menu, wxT("&Help"));

  SetMenuBar(menu_bar);

  CreateStatusBar(2);
  SetStatusText(wxT("Welcome to Manzil!"));
}

void manzil::Frame::OnAbout(wxCommandEvent& event) {
  wxString msg;
  msg.Printf(wxT("Hello and welcome to Manzil %s"), MANZIL_VERSION);
  wxMessageBox(msg, wxT("About Minimal"), wxOK | wxICON_INFORMATION, this);
}

void manzil::Frame::OnQuit(wxCommandEvent& event) {
  Close();
}
