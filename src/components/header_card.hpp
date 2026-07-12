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
#ifndef HEADER_CARD_HPP
#define HEADER_CARD_HPP

#include <wx/wx.h>
#include <cstdint>

class HeaderCard : public wxPanel {

 public:
  enum header_mode : std::uint8_t { READER, DIALOG };

  static HeaderCard* MakeReaderMode(wxWindow* parent, wxString number,
                                    wxString arabic, wxString transliteration,
                                    wxString translation) {
    return new HeaderCard(parent, std::move(number), "", std::move(arabic),
                          std::move(transliteration), std::move(translation),
                          HeaderCard::READER);
  }

  static HeaderCard* MakeDialogMode(wxWindow* parent, wxString number,
                                    wxString ayah, wxString arabic,
                                    wxString transliteration,
                                    wxString translation) {
    return new HeaderCard(parent, std::move(number), std::move(ayah),
                          std::move(arabic), std::move(transliteration),
                          std::move(translation), HeaderCard::DIALOG);
  }

  void SetData(wxString number, wxString arabic, wxString transliteration,
               wxString translation, wxString ayah = "");

 private:
  HeaderCard(wxWindow* parent, wxString number, wxString ayah, wxString arabic,
             wxString transliteration, wxString translation, header_mode mode);
  static constexpr int k_number_fontsize = 14;
  static constexpr int k_name_fontsize = 11;
  static constexpr int k_translation_fontsize = 10;
  static constexpr wxCoord k_padding = 8;

  wxString number_;
  wxString arabic_;
  wxString transliteration_;
  wxString translation_;
  wxString ayah_;
  header_mode mode_;

  void UpdateMaxSize();

  void OnPaint(wxPaintEvent& event);
};

#endif  // HEADER_CARD_HPP
