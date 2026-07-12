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

#ifndef FRAME_HPP
#define FRAME_HPP

#include <wx/wx.h>
#include "reader.hpp"

/**
  * @brief main application window holding loaded Quran data (Quranite), handling menu actions, and toggle between surah list and reader panel.
 */
class Frame : public wxFrame {
 private:
  static constexpr int k_min_width = 400;
  static constexpr int k_min_height = 300;
  static constexpr int k_grid_col_divisor = 250;

  unsigned int surah_number_ =
      1;  ///< Currently displayed surah number (1-114).
  Quranite quranite_;

  HeaderCard* header_card_ =
      nullptr;  ///< Active header above reader panel; null until a surah is opened.
  Reader* reader_ =
      nullptr;  ///< Active reader panel; null until a surah is opened.
  wxScrolledWindow* surah_list_ =
      nullptr;  ///< Scrollable list of SurahCard panels shown on launch.
  wxGridSizer* grid_ = nullptr;

  /**
    * @brief Handle the Quit menu action. 
    * @param event Unused command event.
    */
  void OnQuit(wxCommandEvent& event);

  /**
    * @brief Handle the About menu action.
    * @param event Unused command event.
    */
  void OnAbout(wxCommandEvent& event);

  /**
    * @brief Handle a surah being selected from the surah list.
    *
    * Change visible panel from surah list to reader view
    * and load surah content.
    *
    * @param event Carries the ID of the selected SurahCard.
    */
  void OnSurahSelected(wxCommandEvent& event);

  /**
    * @brief Handle Escape to return from the reader view to the surah list.
    *
    * Bound to wxEVT_CHAR_HOOK rather than wxEVT_KEY_DOWN so Escape is
    * caught even when focus is inside the reader's wxWebView, which
    * would otherwise consume the key first. All other keys are skipped
    * and allowed to propagate normally.
    *
    * @param event The key press event.
 */
  void OnKeyDown(wxKeyEvent& event);

  /**
    * @brief Handle changing grid col when window size change.
    *
    * Col growth decided by k_grid_col_divisor  
    *
    * @param event The size event.
 */
  void OnSize(wxSizeEvent& event);

 public:
  /**
   * @brief Construct the main application window.
   * @param title Window title text.
   * @param min_width Minimum window width in pixels.
   * @param min_height Minimum window height in pixels.
   */
  explicit Frame(const wxString& title, int min_width = k_min_width,
                 int min_height = k_min_height);
  Frame(const Frame&) = delete;
  Frame& operator=(const Frame&) = delete;
};

#endif  // FRAME_HPP
