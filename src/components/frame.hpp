#ifndef N_FRAME_H_
#define N_FRAME_H_

#include <wx/wx.h>

class Frame : public wxFrame {
 private:
  static constexpr int k_min_width = 400;
  static constexpr int k_min_height = 300;
  wxScrolledWindow* surah_list_ = nullptr;
  wxPanel* reader_ = nullptr;

 public:
  explicit Frame(const wxString& title, int min_width = k_min_width,
                 int min_height = k_min_height);

  void OnQuit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);
  void OnSurahSelected(wxCommandEvent& event);
};

#endif  // N_FRAME_H_
