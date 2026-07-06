#include <wx/wx.h>

#include "../events.hpp"
#include "surah_card.hpp"

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
  wxPaintDC dev_ctx(this);
  const wxSize size = GetClientSize();
  const int surahcard_w = size.GetWidth();
  const int surahcard_h = size.GetHeight();

  dev_ctx.SetBackground(wxBrush(GetParent()->GetBackgroundColour()));
  dev_ctx.Clear();

  const wxColour white(255, 255, 255);
  const wxColour gray(200, 200, 200);
  const wxColour black(0, 0, 0);
  constexpr int pen_width = 1;
  constexpr wxCoord rect_x = 0;
  constexpr wxCoord rect_y = 0;
  constexpr double rect_radius = 10;

  dev_ctx.SetPen(wxPen(white, pen_width));
  wxColour brush_colour = hovered_ ? wxColour(gray) : wxColour(white);
  dev_ctx.SetBrush(wxBrush(brush_colour));
  dev_ctx.SetTextForeground(black);

  dev_ctx.DrawRoundedRectangle(rect_x, rect_y, surahcard_w, surahcard_h,
                               rect_radius);

  wxCoord text_start_y = k_padding;

  // this helper lambda is taking text, weight, and size
  // then draw and center it on device contexts
  // this lambda implicitly change dev_ctx and text_y
  // also need this->FromDIP(), and surahcard_w from local
  auto centerpad_text = [&](int font_size, wxFontWeight weight,
                            const wxString& text) {
    dev_ctx.SetFont(wxFont(FromDIP(font_size), wxFONTFAMILY_DEFAULT,
                           wxFONTSTYLE_NORMAL, weight));
    wxSize ext = dev_ctx.GetTextExtent(text);
    dev_ctx.DrawText(text, (surahcard_w - ext.GetWidth()) / 2, text_start_y);
    text_start_y += ext.GetHeight() + k_padding;
  };

  centerpad_text(k_number_fontsize, wxFONTWEIGHT_BOLD, number_);
  centerpad_text(k_name_fontsize, wxFONTWEIGHT_NORMAL,
                 transliteration_ + " | " + arabic_);
  centerpad_text(k_translation_fontsize, wxFONTWEIGHT_BOLD, translation_);

  const wxString verses_str = verses_ + " verses";
  dev_ctx.SetFont(wxFont(FromDIP(k_verses_fontsize), wxFONTFAMILY_DEFAULT,
                         wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
  wxSize verses_ext = dev_ctx.GetTextExtent(verses_str);
  const wxCoord verses_x = (surahcard_w - verses_ext.GetWidth()) / 2;
  const wxCoord verses_y = text_start_y;
  dev_ctx.SetPen(wxPen(gray, pen_width));
  dev_ctx.DrawRoundedRectangle(
      verses_x, verses_y, verses_ext.GetWidth() + k_padding,
      verses_ext.GetHeight() + k_padding, rect_radius / 2);
  dev_ctx.DrawText(verses_str, verses_x + (k_padding / 2),
                   verses_y + (k_padding / 2));
}

void SurahCard::UpdateMinSize() {
  wxClientDC dev_ctx(this);
  wxCoord total_height = k_padding;

  // this helper lambda is taking text, weight, and size
  // then measure it by implicitly changing total_height
  // also implicitly change dev_ctx
  // also need this->FromDIP() from local
  auto measure = [&](int font_size, wxFontWeight weight, const wxString& text) {
    dev_ctx.SetFont(wxFont(FromDIP(font_size), wxFONTFAMILY_DEFAULT,
                           wxFONTSTYLE_NORMAL, weight));
    total_height += dev_ctx.GetTextExtent(text).GetHeight() + k_padding;
  };

  measure(k_number_fontsize, wxFONTWEIGHT_BOLD, number_);
  measure(k_name_fontsize, wxFONTWEIGHT_NORMAL,
          transliteration_ + " | " + arabic_);
  measure(k_translation_fontsize, wxFONTWEIGHT_BOLD, translation_);
  measure(k_verses_fontsize, wxFONTWEIGHT_NORMAL, verses_ + " verses");
  total_height += k_padding;

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
  (void)event;
  wxCommandEvent evt(EVT_SURAH_SELECTED);
  evt.SetInt((wxAtoi(number_)));
  wxPostEvent(wxGetTopLevelParent(this), evt);
}
