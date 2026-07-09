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
#ifndef READER_HPP
#define READER_HPP

#include <wx/webview.h>
#include <wx/wx.h>
#include "quranite.hpp"
#include "reader_dialog.hpp"

/**
 * @brief custom panel for manzil reader.
 *
 * Reader panel show arabic verses with english/arabic,
 * with notes using webview.
 * Reader panel called from main frame and hidden at first.
 *
 * @see Frame
 *
 */
class Reader : public wxPanel {
 public:
  /**
   * @brief Construct the reader panel window.
   * @param parent parent window.
   * @param quranite.
   * @param surah_number current surah number should start at 1.
   */
  Reader(wxWindow* parent, Quranite& quranite, unsigned int surah_number);

  /**
   * @brief Update current surah number then show it.
   * @param surah_number current surah number should start at 1.
   */
  void LoadSurah(unsigned int surah_number);

  Reader(const Reader&) = delete;
  Reader& operator=(const Reader&) = delete;

 private:
  wxWebView* webview_ = nullptr;
  ReaderDialog* dialog_ = nullptr;
  Quranite& quranite_;
  unsigned int surah_number_;

  /**
   * @brief Create ReaderDialog pop-up showing verses with notes.
   *        if ReaderDialog already created, add stack to the pop-up 
   * @param event wxWebView event.
   */
  void OnNoteClicked(wxWebViewEvent& event);

  /**
   * @brief Build html which will be shown and set as webview page.
   * @param quranite.
   * @param surah_number current surah number should start at 1.
   */
  static wxString BuildHtml(const Quranite& quranite,
                            unsigned int surah_number);
};
#endif  // READER_HPP
