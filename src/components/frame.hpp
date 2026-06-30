#ifndef N_FRAME_H_
#define N_FRAME_H_

#include <wx/wx.h>
#include "reader.hpp"

class Frame : public wxFrame {
 private:
  static constexpr int k_min_width = 400;
  static constexpr int k_min_height = 300;
  wxScrolledWindow* surah_list_ = nullptr;
  Reader* reader_ = nullptr;
  uint surah_number_ = 1;
  Quranite quranite_;

 public:
  explicit Frame(const wxString& title, int min_width = k_min_width,
                 int min_height = k_min_height);
  Frame(const Frame&) = delete;
  Frame& operator=(const Frame&) = delete;

  void OnQuit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);
  void OnSurahSelected(wxCommandEvent& event);
};

#endif  // N_FRAME_H_
