#ifndef N_READER_H_
#define N_READER_H_

#include <wx/webview.h>
#include <wx/wx.h>
#include "quranite.hpp"

class Reader : public wxPanel {
 public:
  Reader(wxWindow* parent, Quranite& quranite, uint surah_number);
  void LoadSurah(uint surah_number);

 private:
  wxWebView* webview_;
  Quranite& quranite_;
  wxString BuildHtml(Quranite& quranite, uint surah_number);
};
#endif  // N_READER_H_
