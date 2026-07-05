#ifndef N_MANZIL_H_
#define N_MANZIL_H_

#include <wx/wx.h>

namespace manzil {

class App : public wxApp {
 public:
  bool OnInit() override;
};

}  // namespace manzil
#endif  // N_MANZIL_H_
