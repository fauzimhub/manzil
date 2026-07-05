#include <wx/wx.h>
#include <string>

#include "components/frame.hpp"
#include "manzil.hpp"

using string = std::string;
using std::cerr;
using std::exception;
using std::filesystem::read_symlink;

bool manzil::App::OnInit() {
  string title = "Manzil";

  auto* frame = new Frame(title);

  frame->Show();

  return true;
}

// TODO: only supports Linux via /proc/self/exe;
// look up Windows (GetModuleFileName)
// for cross-platform support.
// forget mac os, i am too poor to have apple device
path manzil::App::GetExecutableDir() {

  string proc_self_exe = "/proc/self/exe";

  try {

    path exe_path = read_symlink(proc_self_exe);
    return exe_path.parent_path();

  } catch (const exception& e) {
    std::cerr << "<< Manzil : Failed to read symlink " << proc_self_exe << "\n"
              << e.what() << "\n";
    return ".";
  }
}
