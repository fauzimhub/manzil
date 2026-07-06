/* manzil.hpp: Bootstrap Manzil, global util function (get executable directory etc).
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

#ifndef MANZIL_HPP
#define MANZIL_HPP

#include <wx/wx.h>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

using path = std::filesystem::path;
using ifstream = std::ifstream;
using string = std::string;
using json = nlohmann::json;

namespace manzil {

/**
 * @brief Bootstrap Manzil.
 */
class App : public wxApp {
 public:
  bool OnInit() override;
};

/**
 * @brief Get the directory containing the running executable.
 *
 * Resolves the executable's location via the /proc/self/exe symlink
 * (linux).
 *
 * @code
 * // Example : locate bundled assets (JSON data files)
 * // relative to the binary rather than the current working directory.
 *
 * path exe_dir = manzil::GetExecutableDir();
 * path assets = exe_dir / "assets" / "chapters-data.json";
 * @endcode
 * 
 * @see Frame::Frame
 *
 * @return Absolute path to the executable's parent directory, or "."
 *         if the symlink could not be resolved.
 *
 * @note Currently Linux-only (relies on /proc/self/exe).
 *
 */
path GetExecutableDir();
}  // namespace manzil
#endif  // MANZIL_HPP
