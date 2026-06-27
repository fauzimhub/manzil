#include <wx/wx.h>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

#include "quranite.hpp"

using string = std::string;
using json = nlohmann::json;
using ifstream = std::ifstream;
using exception = std::exception;
using std::cerr;

manzil::Quranite::Quranite(string& surah_path) {
  json parsed_surah{};
  ifstream file{surah_path};
  if (file.is_open()) {
    try {

      parsed_surah = json::parse(file);

    } catch (exception& e) {
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

  // surah_count is compile time const on quranite.hpp
  constexpr int expected_count = surah_count;
  if (parsed_surah.size() != expected_count) {
    cerr << "<< Manzil: expected " << expected_count << " surahs parsed, got "
         << parsed_surah.size() << " instead" << "\n";
    exit(1);
  }

  for (uint i = 0; i < surah_count; i++) {
    surah foo{};
    foo.number = parsed_surah[i]["number"].get<int>();
    foo.name_arabic = parsed_surah[i]["name_arabic"].get<string>();
    foo.name_translation = parsed_surah[i]["name_translation"].get<string>();
    foo.name_transliteration =
        parsed_surah[i]["name_transliteration"].get<string>();
    foo.verses_count = parsed_surah[i]["verses_count"].get<int>();
    surah_[i] = foo;
  }
}
