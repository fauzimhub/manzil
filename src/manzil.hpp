#ifndef N_MANZIL_H_
#define N_MANZIL_H_

#include <wx/wx.h>

namespace manzil {

class App : public wxApp {
 public:
  bool OnInit() override;
};

class Frame : public wxFrame {
 public:
  Frame(const wxString& title);

  void OnQuit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);

 private:
  DECLARE_EVENT_TABLE();
};

}  // namespace manzil

#endif  // N_MANZIL_H_
