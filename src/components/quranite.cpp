#include "quranite.hpp"
#include <wx/wx.h>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include "../generated/quranite_data.hpp"
#include "../manzil.hpp"

using string = std::string;
using json = nlohmann::json;
using ifstream = std::ifstream;
using exception = std::exception;
using std::cerr;

// TODO: Only structural JSON validity and top-level entry counts are
//       checked. Semantic correctness of the data itself (e.g. correct
//       ayah counts per surah, correct surah ordering, cross-file
//       consistency between verses_count and actual verse entries) is
//       not validated and is assumed correct from the source files.
Quranite::Quranite(const manzil::quranite_data_paths& paths) {
  (void)paths;

  json parsed_surah{};
  json parsed_ar{};
  json parsed_en{};
  json parsed_notes{};

  try {
    parsed_surah = json::parse(k_chapters_data);
    parsed_ar = json::parse(k_ar_data);
    parsed_en = json::parse(k_en_data);
    parsed_notes = json::parse(k_notes_data);
  } catch (const std::exception& e) {
    std::cerr << "<< Manzil: fatal error loading Quran data: " << e.what()
              << "\n";
    std::exit(1);
  }

  /*
   * surah_ 
   *
   * parsed_surah is structure like this: 
   * [{"number":1,"name":{"arabic":"الفاتحة","transliteration":"Al-Fatihah","english":"The Opening"},"versesCount":7}, ...]
   *
   */
  for (unsigned int i = 0; i < manzil::k_surah_count; i++) {
    manzil::surah foo{};
    foo.number = parsed_surah[i]["number"].get<int>();
    foo.name_arabic = parsed_surah[i]["name"]["arabic"].get<string>();
    foo.name_translation = parsed_surah[i]["name"]["english"].get<string>();
    foo.name_transliteration =
        parsed_surah[i]["name"]["transliteration"].get<string>();
    foo.verses_count = parsed_surah[i]["versesCount"].get<int>();
    surah_[i] = foo;
  }

  /*
   * verse_ 
   *
   * parsed_en is structured like this:
   * [[1,1,"In the name of God, the Almighty,<sup>1</sup> the Merciful."], ...]
   *
   * parsed_ar is structured like this:
   * [[1,1,"بِسْمِ ٱللَّـهِ ٱلرَّحْمَـٰنِ ٱلرَّحِيمِ"], ...]
   *
   */
  unsigned int curr_surah_idx = 0;
  manzil::surah_verses verses_in_surah{};

  for (unsigned int i = 0; i < manzil::k_ayah_count; i++) {
    manzil::verse verse{};
    verse.arabic = parsed_ar[i][2].get<string>();
    verse.english = parsed_en[i][2].get<string>();
    verses_in_surah.push_back(verse);

    bool is_last_ayah = (i == manzil::k_ayah_count - 1);
    unsigned int next_surah_idx =
        is_last_ayah ? 0 : parsed_ar[i + 1][0].get<unsigned int>() - 1;
    bool next_ayah_starts_new_surah =
        !is_last_ayah && next_surah_idx != curr_surah_idx;

    if (is_last_ayah || next_ayah_starts_new_surah) {
      verse_[curr_surah_idx] = verses_in_surah;
      verses_in_surah.clear();
      curr_surah_idx += 1;
    }
  }

  /*
   * note_ 
   * 
   * parsed_notes structured like this: 
   * [... , ["1:3:1", "1. Arabic: <i>ra\u1e25m\u0101n</i>. \
     Typically rendered <i>most merciful</i>. \
     See <a>36:23</a> and note thereto, note to <a>1:1</a>, and Notepad I."], ...]
   *
   */
  for (unsigned int i = 0; i < manzil::k_surah_count; i++) {
    note_[i].resize(static_cast<unsigned int>(surah_[i].verses_count));
  }
  for (const auto& entry : parsed_notes) {
    string key = entry[0].get<string>();
    string text = entry[1].get<string>();

    unsigned int surah_n = 0;
    unsigned int ayah_n = 0;
    unsigned int note_n = 0;

    int fields_matched =
        // NOLINTNEXTLINE(bugprone-unchecked-string-to-number-conversion)
        std::sscanf(key.c_str(), "%u:%u:%u", &surah_n, &ayah_n, &note_n);
    if (fields_matched != 3) {
      std::cerr << "<< Manzil: fatal error malformed note key " << key
                << " expected format \"surah:ayah:note\"";
      std::exit(1);
    }

    note_[surah_n - 1][ayah_n - 1].push_back(text);
  }
}
