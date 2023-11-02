#include "console_style.h"

ConsoleStyle::ConsoleStyle(int text_color, int bg_color) {
  text_color_ = text_color;
  bg_color_ = bg_color;
  info_ = 0;
  info_ |= kSetBg | kSetText;
}

ConsoleStyle::ConsoleStyle(int text_color)
    : text_color_(text_color), bg_color_(9) {
  info_ = 0;
  info_ |= kSetText;
}

auto ConsoleStyle::ToStr() const -> std::string {
  std::string str = "\033[";
  if (info_ != 0) {
    if (info_ & kItalic) str += "3;";
    if (info_ & kUnderscore) str += "4;";
    if (info_ & kCrossed) str += "9;";
    if (info_ & kInverseColor) str += "7;";

    if (info_ & kSetText) str += '3' + std::to_string(text_color_) + ';';
    if (info_ & kSetBg) str += '4' + std::to_string(bg_color_) + ';';

    if (info_ & kBold) str += "25";
    if (str.back() == ';') {
      str.back() = 'm';
    } else {
      str += "m";
    }
  } else {
    str += "0m";
  }
  return str;
}

std::ostream& operator<<(std::ostream& os, const ConsoleStyle& palette) {
#ifdef COLORS_OFF
  return os;
#else
  return os << palette.ToStr();
#endif
}

auto ConsoleStyle::SetBgColor(int bg_color) -> void {
  bg_color_ = bg_color;
  info_ |= kSetBg;
}

auto ConsoleStyle::SetTextColor(int text_color) -> void {
  text_color_ = text_color;
  info_ |= kSetText;
}

auto ConsoleStyle::SetUnderscore(bool b) -> void {
  if (b) {
    info_ |= kUnderscore;
  } else {
    info_ &= ~kUnderscore;
  }
}

auto ConsoleStyle::SetCrossed(bool b) -> void {
  if (b) {
    info_ |= kCrossed;
  } else {
    info_ &= ~kCrossed;
  }
}

auto ConsoleStyle::SetItalic(bool b) -> void {
  if (b) {
    info_ |= kItalic;
  } else {
    info_ &= ~kItalic;
  }
}

auto ConsoleStyle::SetInverseColor(bool b) -> void {
  if (b) {
    info_ |= kInverseColor;
  } else {
    info_ &= ~kInverseColor;
  }
}

auto ConsoleStyle::SetBold(bool b) -> void {
  if (b) {
    info_ |= kBold;
  } else {
    info_ &= ~kBold;
  }
}

ConsoleStyle::ConsoleStyle(ConsoleStyle&& o) noexcept { *this = std::move(o); }
auto ConsoleStyle::operator=(ConsoleStyle&& o) noexcept -> ConsoleStyle& {
  if (this != &o) {
    text_color_ = o.text_color_;
    bg_color_ = o.bg_color_;
    info_ = o.info_;
    o.info_ = 0;
    o.text_color_ = o.bg_color_ = 0;
  }
  return *this;
}
