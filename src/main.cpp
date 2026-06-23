#include <nlohmann/json.hpp>
#include <wx/wx.h>

class MyApp : public wxApp {
public:
  bool OnInit() override {
    nlohmann::json json = {{"name", "manzil"}, {"version", 1}};

    auto *frame =
        new wxFrame(nullptr, wxID_ANY, json["name"].get<std::string>());
    frame->Show();
    return true;
  }
};

wxIMPLEMENT_APP(MyApp);
