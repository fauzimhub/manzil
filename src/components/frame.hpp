/* frame.hpp: main top-level window holding loaded Quran data (Quranite), handling menu actions (quit, about), and toggle between surah list and reader panel.

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

#ifndef FRAME_HPP
#define FRAME_HPP

#include <wx/wx.h>
#include "reader.hpp"

class Frame : public wxFrame {
 private:
  static constexpr int k_min_width = 400;
  static constexpr int k_min_height = 300;

  uint surah_number_ = 1;
  Quranite quranite_;

  Reader* reader_ = nullptr;
  wxScrolledWindow* surah_list_ = nullptr;

  void OnQuit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);
  void OnSurahSelected(wxCommandEvent& event);
  void OnKeyDown(wxKeyEvent& event);

 public:
  explicit Frame(const wxString& title, int min_width = k_min_width,
                 int min_height = k_min_height);
  Frame(const Frame&) = delete;
  Frame& operator=(const Frame&) = delete;
};

#endif  // FRAME_HPP
