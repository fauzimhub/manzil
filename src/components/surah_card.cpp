#include <wx/wx.h>

#include <unordered_map>
#include "../events.hpp"
#include "surah_card.hpp"

using font_map = std::unordered_map<wxString, wxFont>;

SurahCard::SurahCard(wxWindow* parent, wxString number, wxString arabic,
                     wxString transliteration, wxString translation,
                     wxString verses)
    : wxPanel(parent, wxID_ANY),
      number_(std::move(number)),
      arabic_(std::move(arabic)),
      transliteration_(std::move(transliteration)),
      translation_(std::move(translation)),
      verses_(std::move(verses)) {

  UpdateMinSize();
  SetCursor(wxCursor(wxCURSOR_HAND));

  Bind(wxEVT_PAINT, &SurahCard::OnPaint, this);
  Bind(wxEVT_ENTER_WINDOW, &SurahCard::OnEnter, this);
  Bind(wxEVT_LEAVE_WINDOW, &SurahCard::OnLeave, this);
  Bind(wxEVT_LEFT_UP, &SurahCard::OnLeftUp, this);
}

void SurahCard::OnPaint(wxPaintEvent& event) {
  (void)event;
  wxPaintDC device_contexts(this);
  const wxSize size = GetClientSize();
  const int surahcard_w = size.GetWidth();
  const int surahcard_h = size.GetHeight();

  device_contexts.SetBackground(wxBrush(GetParent()->GetBackgroundColour()));
  device_contexts.Clear();

  const wxColour white(255, 255, 255);
  const wxColour gray(200, 200, 200);
  const wxColour black(0, 0, 0);
  constexpr int pen_width = 1;
  constexpr wxCoord rect_x = 0;
  constexpr wxCoord rect_y = 0;
  constexpr double rect_radius = 10;

  device_contexts.SetPen(wxPen(white, pen_width));
  wxColour brush_colour = hovered_ ? wxColour(gray) : wxColour(white);
  device_contexts.SetBrush(wxBrush(brush_colour));
  device_contexts.SetTextForeground(black);

  device_contexts.DrawRoundedRectangle(rect_x, rect_y, surahcard_w, surahcard_h,
                                       rect_radius);

  wxCoord text_y = padding_;

  auto centerpad_text = [&](const wxString& text) {
    wxSize ext = device_contexts.GetTextExtent(text);
    device_contexts.DrawText(text, (surahcard_w - ext.GetWidth()) / 2, text_y);
    text_y += ext.GetHeight() + padding_;
  };

  device_contexts.SetFont(wxFont(FromDIP(number_fontsize_),
                                 wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
                                 wxFONTWEIGHT_BOLD));
  centerpad_text(number_);

  device_contexts.SetFont(wxFont(FromDIP(name_fontsize_), wxFONTFAMILY_DEFAULT,
                                 wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
  centerpad_text(transliteration_ + " | " + arabic_);

  device_contexts.SetFont(wxFont(FromDIP(translation_fontsize_),
                                 wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
                                 wxFONTWEIGHT_BOLD));
  centerpad_text(translation_);

  const wxString verses_str = verses_ + " verses";
  device_contexts.SetFont(wxFont(FromDIP(verses_fontsize_),
                                 wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
                                 wxFONTWEIGHT_NORMAL));
  wxSize vext = device_contexts.GetTextExtent(verses_str);
  const wxCoord verses_x = (surahcard_w - vext.GetWidth()) / 2;
  const wxCoord verses_y = text_y;
  device_contexts.SetPen(wxPen(gray, pen_width));
  device_contexts.DrawRoundedRectangle(
      verses_x, verses_y, vext.GetWidth() + padding_,
      vext.GetHeight() + padding_, rect_radius / 2);
  device_contexts.DrawText(verses_str, verses_x + (padding_ / 2),
                           verses_y + (padding_ / 2));
}

void SurahCard::UpdateMinSize() {
  wxClientDC device_contexts(this);
  wxCoord total_height = padding_;

  auto measure = [&](int font_size, wxFontWeight weight, const wxString& text) {
    device_contexts.SetFont(wxFont(FromDIP(font_size), wxFONTFAMILY_DEFAULT,
                                   wxFONTSTYLE_NORMAL, weight));
    total_height += device_contexts.GetTextExtent(text).GetHeight() + padding_;
  };

  measure(number_fontsize_, wxFONTWEIGHT_BOLD, number_);
  measure(name_fontsize_, wxFONTWEIGHT_NORMAL,
          transliteration_ + " | " + arabic_);
  measure(translation_fontsize_, wxFONTWEIGHT_BOLD, translation_);
  measure(verses_fontsize_, wxFONTWEIGHT_NORMAL, verses_ + " verses");
  total_height += padding_;

  SetMinSize(wxSize(wxDefaultCoord, total_height));
}

void SurahCard::OnEnter(wxMouseEvent& event) {
  (void)event;
  hovered_ = true;
  Refresh();
}

void SurahCard::OnLeave(wxMouseEvent& event) {
  (void)event;
  hovered_ = false;
  Refresh();
}

void SurahCard::OnLeftUp(wxMouseEvent& event) {
  wxCommandEvent evt(EVT_SURAH_SELECTED);
  evt.SetEventObject(this);
  wxPostEvent(GetParent(), evt);
}
