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
  json data{};
  string title = "no title";

  ifstream file{"assets/surah.json"};
  if (file) {
    data = json::parse(file);
    title = data[0]["name_translation"].get<string>();
    file.close();
  }

  cout << data << "\n";

  auto* frame = new wxFrame(nullptr, wxID_ANY, title);

  frame->Show();

  return true;
}
