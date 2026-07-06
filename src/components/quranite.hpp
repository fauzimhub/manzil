/*
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
#ifndef QURANITE_HPP
#define QURANITE_HPP

#include <fstream>
#include <string>
#include "../types.hpp"

using string = std::string;
using ifstream = std::ifstream;

/**
 * @brief Loads and holds all Quranite data: surah metadata, verse text,
 *        and footnotes, indexed by surah and ayah number.
 *
 * @warning Construction is all-or-nothing, if any source data file
 *          fails to load or parse, the process terminates via
 *          exit(1) rather than leaving a partially-loaded instance.
 */
class Quranite {
 private:
  manzil::surahs_by_number
      surah_{};  ///< Metadata (name, verse count) for each surah, indexed by surah number.
  manzil::verses_by_surah
      verse_{};  ///< Arabic/English verse text, indexed by surah then ayah.
  manzil::notes_by_surah
      note_{};  ///< Footnotes, indexed by surah then ayah then note number.

 public:
  explicit Quranite(manzil::quranite_data_paths& paths);

  [[nodiscard]] const manzil::surahs_by_number& GetSurah() const {
    return surah_;
  }

  [[nodiscard]] const manzil::verses_by_surah& GetVerse() const {
    return verse_;
  }

  [[nodiscard]] const manzil::notes_by_surah& GetNote() const { return note_; }
};

#endif  // QURANITE_HPP
