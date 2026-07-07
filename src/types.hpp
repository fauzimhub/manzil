/* types.hpp: types and global constant, unique to manzil (surah, verse, notes).  
Copyright (c) 2026 Maher Fauzi 
   Permission is hereby granted, free of charge, to any person obtaining a copy of
   this software and associated documentation files (the "Software"), to deal in
   the Software without restriction, including without limitation the rights to
   use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
   of the Software, and to permit persons to whom the Software is furnished to do
   so, subject to the following conditions:
   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/
#ifndef TYPES_HPP
#define TYPES_HPP

#include <array>
#include <string>
#include <vector>

namespace manzil {

/**
 * @brief The set of file paths Quranite needs to load its data, one
 *        JSON file each for surah metadata, Arabic verses, English
 *        verses with notes, and standalone footnotes.
 *
 * @warning The "placeholder" default values should never appear in
 *          practice, they indicate the paths were never assigned
 *          before being passed to Quranite's constructor.
 */
struct quranite_data_paths {
  std::string surah = "placeholder";
  std::string ar = "placeholder";
  std::string en = "placeholder";
  std::string notes = "placeholder";
};

/**
 * @brief parameter used mainly for pop-up verse 
 *
 * @see ReaderDialog
 *
 * @warning Default member value 0 are placeholders only
 *          and should never appear in practice, they indicate a
 *          entry failed to be initialized.
 */
struct nav_entry {
  uint surah = 0;
  uint begin_ayah = 0;
  uint end_ayah = 0;
};

/**
 * @brief Metadata for a single surah (chapter) of the Quran.
 *
 * @warning Default member values ("noname", 0) are placeholders only
 *          and should never appear in practice, they indicate a
 *          surah that failed to load correctly from chapters-data.json.
 */
struct surah {
  int number = 0;
  std::string name_arabic = "noname";
  std::string name_transliteration = "noname";
  std::string name_translation = "noname";
  int verses_count = 0;
};

/**
 * @brief A single verse (ayah), paired in Arabic and English.
 *
 * @warning The "placeholder" default values should never appear in
 *          practice, they indicate a verse that failed to load
 *          correctly from the source JSON files.
 */
struct verse {
  std::string arabic = "placeholder";
  std::string english = "placeholder";
};

constexpr uint k_surah_count = 114;
constexpr uint k_ayah_count = 6236;

using surahs_by_number = std::array<surah, k_surah_count>;

using surah_verses = std::vector<verse>;
using verses_by_surah = std::array<surah_verses, k_surah_count>;

using ayah_notes = std::vector<std::string>;
using surah_notes = std::vector<ayah_notes>;
using notes_by_surah = std::array<surah_notes, k_surah_count>;
}  // namespace manzil

#endif  //TYPES_HPP
