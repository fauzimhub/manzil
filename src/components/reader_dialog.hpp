#ifndef N_READER_DIALOG_H_
#define N_READER_DIALOG_H_

#include <wx/webview.h>
#include <wx/wx.h>
#include <vector>
#include "quranite.hpp"

class ReaderDialog : public wxDialog {
 public:
  ReaderDialog(wxWindow* parent, Quranite& quranite, uint surah,
               uint begin_ayah, uint end_ayah);
  void Navigate(uint surah, uint begin_ayah, uint end_ayah);

 private:
  struct NavEntry {
    uint surah;
    uint begin_ayah;
    uint end_ayah;
  };

  Quranite& quranite_;
  wxWebView* webview_;
  wxButton* back_btn_;
  std::vector<NavEntry> history_;

  wxString BuildHtml(uint surah, uint begin_ayah, uint end_ayah);
  void OnBack(wxCommandEvent& event);
  void OnVerseRef(wxWebViewEvent& event);
};

#endif  // N_READER_DIALOG_H_
