#include <wx/wx.h>
#include <iostream>
#include <string>

#include "manzil.hpp"
#include "quranite.hpp"

using string = std::string;
using std::cout;

bool manzil::App::OnInit() {
  string title = "Manzil";
  string surah_path = "assets/surah.json";

  auto* quranite = new manzil::Quranite(surah_path);

  for (const auto& sur : quranite->getSurah()) {
    cout << "Surah (Number: " << sur.number
         << ", Name Arabic: " << sur.name_arabic
         << ", Name Translation: " << sur.name_translation
         << ", Name Transliteration: " << sur.name_transliteration
         << ", Verses Count: " << sur.verses_count << ")" << "\n";
  }

  auto* frame = new wxFrame(nullptr, wxID_ANY, title);

  frame->Show();

  return true;
}
