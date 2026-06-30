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

Quranite::Quranite(string& surah_path, string& verses_ar, string& verses_en) {
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
  constexpr int expected_count = manzil::k_surah_count;
  if (parsed_surah.size() != expected_count) {
    cerr << "<< Manzil: expected " << expected_count << " surahs parsed, got "
         << parsed_surah.size() << " instead" << "\n";
    exit(1);
  }

  for (uint i = 0; i < manzil::k_surah_count; i++) {
    manzil::surah foo{};
    foo.number = parsed_surah[i]["number"].get<int>();
    foo.name_arabic = parsed_surah[i]["name"]["arabic"].get<string>();
    foo.name_translation = parsed_surah[i]["name"]["english"].get<string>();
    foo.name_transliteration =
        parsed_surah[i]["name"]["transliteration"].get<string>();
    foo.verses_count = parsed_surah[i]["versesCount"].get<int>();
    surah_[i] = foo;
  }

  json parsed_ar{};
  ifstream file_ar{verses_ar};
  if (file_ar.is_open()) {
    try {

      parsed_ar = json::parse(file_ar);

    } catch (exception& e) {
      cerr << "<< Manzil: encountered an error while trying to parse \""
           << verses_ar << "\"" << " as json" << ": "
           << "\n"
           << e.what() << "\n";
      exit(1);
    }
  } else {
    cerr << "<< Manzil: could not open \"" << verses_ar << "\"\n";
    exit(1);
  }
  constexpr int expected_ayah = manzil::k_ayah_count;
  if (parsed_ar.size() != expected_ayah) {
    cerr << "<< Manzil: expected " << expected_ayah << " ayah parsed, got "
         << parsed_ar.size() << " instead" << "\n";
    exit(1);
  }

  json parsed_en{};
  ifstream file_en{verses_en};
  if (file_en.is_open()) {
    try {

      parsed_en = json::parse(file_en);

    } catch (exception& e) {
      cerr << "<< Manzil: encountered an error while trying to parse \""
           << verses_en << "\"" << " as json" << ": "
           << "\n"
           << e.what() << "\n";
      exit(1);
    }
  } else {
    cerr << "<< Manzil: could not open \"" << verses_en << "\"\n";
    exit(1);
  }
  if (parsed_en.size() != expected_ayah) {
    cerr << "<< Manzil: expected " << expected_ayah << " ayah parsed, got "
         << parsed_en.size() << " instead" << "\n";
    exit(1);
  }

  uint surah_num = 1;
  manzil::verse_v foo_v{};
  for (uint i = 0; i < manzil::k_ayah_count; i++) {
    manzil::verse foo{};
    auto current_num = parsed_ar[i][0].get<uint>();
    if (current_num != surah_num) {
      verse_[surah_num] = foo_v;
      surah_num += 1;
      foo_v.clear();
    }
    foo.arabic = parsed_ar[i][2].get<string>();
    foo.english = parsed_en[i][2].get<string>();
    foo_v.emplace_back(foo);
  }
}
