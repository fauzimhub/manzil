#ifndef N_TYPES_H_
#define N_TYPES_H_

#include <array>
#include <string>
#include <vector>

namespace manzil {
struct surah {
  int number = 0;
  std::string name_arabic = "noname";
  std::string name_transliteration = "noname";
  std::string name_translation = "noname";
  int verses_count = 0;
};

struct verse {
  std::string arabic = "placeholder";
  std::string english = "placeholder";
};

constexpr int k_surah_count = 114;
constexpr int k_ayah_count = 6236;

using surah_list = std::array<surah, k_surah_count>;

using verse_v = std::vector<verse>;
using verse_list = std::array<verse_v, k_surah_count>;

using note_v = std::vector<std::string>;
using ayah_notes = std::vector<note_v>;
using note_list = std::array<ayah_notes, k_surah_count>;
}  // namespace manzil

#endif
