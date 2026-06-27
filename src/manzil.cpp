#include <wx/wx.h>
#include <string>

#include "components/frame.hpp"
#include "manzil.hpp"

using string = std::string;

bool manzil::App::OnInit() {
  string title = "Manzil";

  auto* frame = new Frame(title);

  frame->Show();

  return true;
}
