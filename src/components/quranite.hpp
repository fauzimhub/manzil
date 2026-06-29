#ifndef N_QURANITE_H_
#define N_QURANITE_H_

#include <string>
#include "../types.hpp"

using string = std::string;

class Quranite {
 private:
  manzil::surah_list surah_{};

 public:
  explicit Quranite(string& surah_path);

  [[nodiscard]] const manzil::surah_list& getSurah() const { return surah_; }
};

#endif  // N_QURANITE_H_
