#ifndef N_QURANITE_H_
#define N_QURANITE_H_

#include <string>
#include "../types.hpp"

using string = std::string;

class Quranite {
 private:
  manzil::surah_list surah_{};
  manzil::verse_list verse_{};
  manzil::note_list note_{};

 public:
  explicit Quranite(const string& surah_path, const string& verses_ar,
                    const string& verses_en, const string& notes_path);

  [[nodiscard]] const manzil::surah_list& getSurah() const { return surah_; }

  [[nodiscard]] const manzil::verse_list& getVerse() const { return verse_; }

  [[nodiscard]] const manzil::note_list& getNote() const { return note_; }
};

#endif  // N_QURANITE_H_
