#include <wx/wx.h>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

#include "quranite.hpp"

using string = std::string;
using json = nlohmann::json;
using ifstream = std::ifstream;
using std::cerr;

manzil::Quranite::Quranite(string& surah_path) {
  json parsed_surah{};
  ifstream file{surah_path};
  if (file.is_open()) {
    try {

      parsed_surah = json::parse(file);

    } catch (std::exception& e) {
      cerr << "<< Manzil: encountered an error while trying to parse \""
           << surah_path << "\"" << " as json" << ": "
           << "\n"
           << e.what() << "\n";
      exit(1);
    }
  } else {
    cerr << "<< Manzil: could not open \"" << surah_path << "\"\n";
    exit(1);
  }

  for (const auto& parsed : parsed_surah) {
    surah foo{};
    foo.number = parsed["number"].get<int>();
    foo.name_arabic = parsed["name_arabic"].get<string>();
    foo.name_translation = parsed["name_translation"].get<string>();
    foo.name_transliteration = parsed["name_transliteration"].get<string>();
    foo.verses_count = parsed["verses_count"].get<int>();
    surah_.push_back(foo);
  }
}
