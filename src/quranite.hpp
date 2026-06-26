#ifndef N_QURANITE_H_
#define N_QURANITE_H_

#include <array>
#include <string>

using string = std::string;

struct surah {
  int number = 0;
  string name_arabic = "noname";
  string name_transliteration = "noname";
  string name_translation = "noname";
  int verses_count = 0;
};

constexpr int surah_count = 114;
using surah_list = std::array<surah, surah_count>;

namespace manzil {

class Quranite {
 private:
  surah_list surah_{};

 public:
  explicit Quranite(string& surah_path);

  [[nodiscard]] const surah_list& getSurah() const { return surah_; }
};

}  // namespace manzil
#endif  // N_QURANITE_H_
