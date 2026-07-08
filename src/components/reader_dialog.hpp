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
#ifndef READER_DIALOG_HPP
#define READER_DIALOG_HPP

#include <wx/webview.h>
#include <wx/wx.h>
#include <vector>
#include "../types.hpp"
#include "quranite.hpp"

/**
 * @brief custom dialog for manzil pop-up.
 *
 * Reader dialog is a stack-based pop-up called from reader panel,
 * show verse or verses with notes.
 * This dialog doesnt lock reader panel, so user can add more
 * stack to pop-up through clicking reference in a reader panel. 
 * Notes in this pop-up may contain reference that can be clicked 
 * to add more stack to pop-up.
 *
 */
class ReaderDialog : public wxDialog {
 public:
  /**
   * @brief Construct the reader dialog window.
   * @param parent parent window.
   * @param quranite.
   * @param entry struct entry contain surah number, begin and end ayah.
   */
  ReaderDialog(wxWindow* parent, Quranite& quranite, manzil::nav_entry entry);

  /**
   * @brief Add entry to pop-up stack.
   * @param entry struct entry contain surah number, begin and end ayah.
   */
  void Navigate(manzil::nav_entry entry);

  ReaderDialog(const ReaderDialog&) = delete;
  ReaderDialog& operator=(const ReaderDialog&) = delete;

 private:
  Quranite& quranite_;
  wxWebView* webview_ = nullptr;
  wxButton* back_btn_ = nullptr;
  std::vector<manzil::nav_entry> history_{
      0};  ///< contain stack of opened entry

  wxString BuildHtml(manzil::nav_entry entry);

  /**
    * @brief Handle when back button in dialog is clicked.
    * @param event Unused command event.
    */
  void OnBack(wxCommandEvent& event);

  /**
    * @brief Handle when reference in dialog note is clicked.
    * @param event The wxWebView event.
    */
  void OnVerseRef(wxWebViewEvent& event);
};

#endif  // READER_DIALOG_HPP
