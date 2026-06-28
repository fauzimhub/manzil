#ifndef N_SURAH_CARD_H_
#define N_SURAH_CARD_H_

#include <wx/wx.h>
#include <unordered_map>

using font_map = std::unordered_map<wxString, wxFont>;

class SurahCard : public wxPanel {
 public:
  SurahCard(wxWindow* parent, wxString number, wxString arabic,
            wxString transliteration, wxString translation, wxString verses);
  void OnPaint(wxPaintEvent& event);
  void OnEnter(wxMouseEvent& event);
  void OnLeave(wxMouseEvent& event);
  void OnLeftUp(wxMouseEvent& event);

 private:
  void UpdateMinSize();
  static constexpr int number_fontsize_ = 14;
  static constexpr int name_fontsize_ = 11;
  static constexpr int translation_fontsize_ = 10;
  static constexpr int verses_fontsize_ = 8;
  static constexpr wxCoord padding_ = 8;
  bool hovered_ = false;
  wxString number_;
  wxString arabic_;
  wxString transliteration_;
  wxString translation_;
  wxString verses_;
};
#endif  // N_SURAH_CARD_H_
