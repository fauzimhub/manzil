#ifndef N_QURANITE_H_
#define N_QURANITE_H_

#include <string>
#include <vector>

using string = std::string;

struct surah {
  int number;
  string name_arabic;
  string name_transliteration;
  string name_translation;
  int verses_count;
};

using vsurah = std::vector<surah>;

namespace manzil {

class Quranite {
 private:
  vsurah surah_;

 public:
  Quranite(string surah_path);

  vsurah getSurah() { return surah_; }
};

}  // namespace manzil
#endif  // N_QURANITE_H_
