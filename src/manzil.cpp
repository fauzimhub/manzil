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

namespace manzil {

// TODO: only supports Linux via /proc/self/exe;
//       look up Windows (GetModuleFileName)
//       for cross-platform support.
//       forget mac os, i am too poor to have apple device
path GetExecutableDir() {
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

// TODO: rather than runtime_error
//       it might be worth it to make
//       my own error type.
//       but its not worth it yet currently,
//       the priority is low.
json ParseJSON(const string& path) {
  ifstream file{path};
  if (!file.is_open()) {
    throw std::runtime_error("could not open \"" + path + "\"");
  }
  try {
    return json::parse(file);
  } catch (const json::parse_error& e) {
    throw std::runtime_error("failed to parse \"" + path +
                             "\" as json: " + e.what());
  }
}

json ParseJSON(const string& path, size_t expected_size) {
  json parsed_file = ParseJSON(path);
  if (parsed_file.size() != expected_size) {
    throw std::runtime_error("expected " + std::to_string(expected_size) +
                             " entries in \"" + path + "\", got " +
                             std::to_string(parsed_file.size()));
  }
  return parsed_file;
}

}  // namespace manzil
