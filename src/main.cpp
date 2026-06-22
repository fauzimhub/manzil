#include <wx/wx.h>

class MyApp : public wxApp {
public:
  bool OnInit() override {
    auto *frame =
        new wxFrame(nullptr, wxID_ANY, "All your codebase are belong to us");
    frame->Show();
    return true;
  }
};

wxIMPLEMENT_APP(MyApp);
