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

  auto* frame = new wxFrame(nullptr, wxID_ANY, title);

  frame->Show();

  return true;
}
