#ifndef N_QURANITE_H_
#define N_QURANITE_H_

#include <string>
#include <vector>

using string = std::string;

struct surah {
  int number = 0;
  string name_arabic = "noname";
  string name_transliteration = "noname";
  string name_translation = "noname";
  int verses_count = 0;
};

using vsurah = std::vector<surah>;

namespace manzil {

class Quranite {
 private:
  // empty; size explicitly 0 to satisfy -Weffc++ and clang-tidy
  vsurah surah_{0};

 public:
  explicit Quranite(string& surah_path);

  [[nodiscard]] const vsurah& getSurah() const { return surah_; }
};

}  // namespace manzil
#endif  // N_QURANITE_H_
