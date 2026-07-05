/* main.cpp: set environment variable, set manzil instance.

Copyright (c) 2026 Maher Fauzi 
   Permission is hereby granted, free of charge, to any person obtaining a copy of
   this software and associated documentation files (the "Software"), to deal in
   the Software without restriction, including without limitation the rights to
   use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
   of the Software, and to permit persons to whom the Software is furnished to do
   so, subject to the following conditions:
   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.

 */

#include <wx/wx.h>
#include "manzil.hpp"

int main(int argc, char** argv) {

  /*
   * on linux without setting disabling dmabuf renderer got : 
   * error 71(Protocol error) dispatching to Wayland
   * a.k.a black screen the exit on wxwidgets web view.
   * havent quite found the reason yet, 
   * something with nvidia maybe ?  
   * trying GSK_RENDERER=gl, GSK_RENDERER=ngl change nothing.
   * using GDK_BACKEND=x11 on wayland got me different error:  
   * Failed to create GBM buffer of size 400x273: Invalid argument
   * what a mess...
   */
#ifdef _WIN32
#else
  setenv("WEBKIT_DISABLE_DMABUF_RENDERER", "1", 1);
#endif

  wxApp::SetInstance(new manzil::App());
  return wxEntry(argc, argv);
}
