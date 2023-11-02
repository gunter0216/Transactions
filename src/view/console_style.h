#ifndef A6_CONSOLE_STYLE_H
#define A6_CONSOLE_STYLE_H

#ifdef WIN32
#define COLORS_OFF
#endif
#ifdef WIN64
#define COLORS_OFF
#endif
#ifdef WINNT
#define COLORS_OFF
#endif

#include <ostream>

class ConsoleStyle {
 public:
  ConsoleStyle() = default;
  ConsoleStyle(int text_color, int bg_color);
  explicit ConsoleStyle(int text_color);
  inline ConsoleStyle(const ConsoleStyle& o)
      : info_(o.info_), text_color_(o.text_color_), bg_color_(o.bg_color_){};
  ConsoleStyle(ConsoleStyle&& o) noexcept;
  auto operator=(const ConsoleStyle& o) -> ConsoleStyle& = default;
  auto operator=(ConsoleStyle&& o) noexcept -> ConsoleStyle&;
  ~ConsoleStyle() = default;

  inline auto GetInfo() const -> char { return info_; };
  inline auto GetTextColor() const -> int { return text_color_; }
  auto SetTextColor(int text_color) -> void;
  inline auto GetBgColor() const -> int { return bg_color_; }
  auto SetBgColor(int bg_color) -> void;

  auto SetUnderscore(bool b = true) -> void;
  auto SetCrossed(bool b = true) -> void;
  auto SetItalic(bool b = true) -> void;
  auto SetInverseColor(bool b = true) -> void;
  auto SetBold(bool b = true) -> void;

  auto ClearText() -> void { info_ &= ~kSetText; }
  auto ClearBg() -> void { info_ &= ~kSetBg; }
  auto Clear() -> void { info_ = 0; }

  inline auto Underscore() const -> bool { return info_ & kUnderscore; };
  inline auto Crossed() const -> bool { return info_ & kCrossed; };
  inline auto Italic() const -> bool { return info_ & kItalic; };
  inline auto InverseColor() const -> bool { return info_ & kInverseColor; };
  inline auto Bold() const -> bool { return info_ & kBold; };

  auto ToStr() const -> std::string;

  friend std::ostream& operator<<(std::ostream& os,
                                  const ConsoleStyle& palette);

 private:
  enum Info : unsigned char {
    kSetText = 0b01000000,
    kSetBg = 0b00100000,
    kUnderscore = 0b00010000,    // 4
    kCrossed = 0b00001000,       // 9
    kItalic = 0b00000100,        // 3
    kInverseColor = 0b00000010,  // 7
    kBold = 0b00000001           // 25m/20m
  };

  char info_{};
  int text_color_{9};
  int bg_color_{9};
};

constexpr ConsoleStyle ClearStyle = ConsoleStyle();

#endif  // A6_CONSOLE_STYLE_H
