#ifndef N_READER_H_
#define N_READER_H_

#include <wx/webview.h>
#include <wx/wx.h>
#include "quranite.hpp"

class Reader : public wxPanel {
 public:
  Reader(wxWindow* parent, Quranite& quranite, uint surah_number);
  void LoadSurah(uint surah_number);
  void OnNoteClicked(wxWebViewEvent& event);

  Reader(const Reader&) = delete;
  Reader& operator=(const Reader&) = delete;

 private:
  wxWebView* webview_ = nullptr;
  Quranite& quranite_;
  uint surah_number_;
  static wxString BuildHtml(const Quranite& quranite, uint surah_number);
};
#endif  // N_READER_H_
