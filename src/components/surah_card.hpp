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
#ifndef SURAH_CARD_HPP
#define SURAH_CARD_HPP

#include <wx/wx.h>

/**
 * @brief custom panel acting as button containing surah metadata.
 *
 * contain number surah, name surah in arabic, transliteration,
 * and name translation in english, also verses count of surah.
 *
 */
class SurahCard : public wxPanel {
 public:
  SurahCard(wxWindow* parent, wxString number, wxString arabic,
            wxString transliteration, wxString translation, wxString verses);

 private:
  static constexpr int k_number_fontsize = 14;
  static constexpr int k_name_fontsize = 11;
  static constexpr int k_translation_fontsize = 10;
  static constexpr int k_verses_fontsize = 8;
  static constexpr wxCoord k_padding = 8;

  bool hovered_ = false;

  wxString number_;
  wxString arabic_;
  wxString transliteration_;
  wxString translation_;
  wxString verses_;

  void UpdateMinSize();

  void OnPaint(wxPaintEvent& event);
  void OnEnter(wxMouseEvent& event);
  void OnLeave(wxMouseEvent& event);
  void OnLeftUp(wxMouseEvent& event);
};
#endif  // SURAH_CARD_HPP
