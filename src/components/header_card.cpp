#include "header_card.hpp"

HeaderCard::HeaderCard(wxWindow* parent, wxString number, wxString ayah,
                       wxString arabic, wxString transliteration,
                       wxString translation, header_mode mode)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
              wxFULL_REPAINT_ON_RESIZE),
      number_(std::move(number)),
      ayah_(std::move(ayah)),
      arabic_(std::move(arabic)),
      transliteration_(std::move(transliteration)),
      translation_(std::move(translation)) {
  mode_ = mode;
  UpdateMaxSize();

  Bind(wxEVT_PAINT, &HeaderCard::OnPaint, this);
}

void HeaderCard::OnPaint(wxPaintEvent& event) {
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

  dev_ctx.SetPen(wxPen(white, pen_width));
  wxColour brush_colour = wxColour(white);
  dev_ctx.SetBrush(wxBrush(brush_colour));
  dev_ctx.SetTextForeground(black);

  dev_ctx.DrawRectangle(rect_x, rect_y, surahcard_w, surahcard_h);

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

  if (mode_ == HeaderCard::READER) {
    centerpad_text(k_number_fontsize, wxFONTWEIGHT_BOLD, number_);
    centerpad_text(k_name_fontsize, wxFONTWEIGHT_NORMAL,
                   transliteration_ + " | " + arabic_);
    centerpad_text(k_translation_fontsize, wxFONTWEIGHT_BOLD, translation_);
  }

  else if (mode_ == HeaderCard::DIALOG) {
    centerpad_text(k_name_fontsize, wxFONTWEIGHT_NORMAL,
                   transliteration_ + " | " + translation_ + " | " + arabic_);

    centerpad_text(k_name_fontsize, wxFONTWEIGHT_NORMAL,
                   number_ + " : " + ayah_);
  }

  else {
    // TODO: UNKNOWN MODE
  }
}

void HeaderCard::UpdateMaxSize() {
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

  if (mode_ == HeaderCard::READER) {
    measure(k_number_fontsize, wxFONTWEIGHT_BOLD, number_);
    measure(k_name_fontsize, wxFONTWEIGHT_NORMAL,
            transliteration_ + " | " + arabic_);
    measure(k_translation_fontsize, wxFONTWEIGHT_BOLD, translation_);
  }

  else if (mode_ == HeaderCard::DIALOG) {
    measure(k_name_fontsize, wxFONTWEIGHT_NORMAL,
            transliteration_ + " | " + translation_ + " | " + arabic_);

    measure(k_name_fontsize, wxFONTWEIGHT_NORMAL, number_ + " : " + ayah_);
  }

  else {
    // TODO: UNKNOWN MODE
  }

  total_height += k_padding;

  SetMaxSize(wxSize(wxDefaultCoord, total_height));
}

void HeaderCard::SetData(wxString number, wxString arabic,
                         wxString transliteration, wxString translation,
                         wxString ayah) {
  number_ = std::move(number);
  ayah_ = std::move(ayah);
  arabic_ = std::move(arabic);
  transliteration_ = std::move(transliteration);
  translation_ = std::move(translation);
  Refresh();
}
