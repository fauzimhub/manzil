// main.cpp
#include <wx/wx.h>
#include <cstdlib>
#include "manzil.hpp"

int main(int argc, char** argv) {
  setenv("WEBKIT_DISABLE_DMABUF_RENDERER", "1", 1);
  wxApp::SetInstance(new manzil::App());
  return wxEntry(argc, argv);
}
