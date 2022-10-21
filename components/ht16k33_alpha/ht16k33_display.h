#pragma once

#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/components/i2c/i2c.h"

#ifdef USE_TIME
#include "esphome/components/time/real_time_clock.h"
#endif

namespace esphome {
namespace ht16k33_alpha {

class HT16K33AlphaDisplay : public PollingComponent, public i2c::I2CDevice {
 public:
  void set_writer(std::function<void(HT16K33AlphaDisplay &)> &&writer) { this->writer_ = std::move(writer); }
  void setup() override;
  void loop() override;
  float get_setup_priority() const override;
  void add_secondary_display(i2c::I2CDevice *display) { this->displays_.push_back(display); }
  void set_scroll(bool scroll) { this->scroll_ = scroll; }
  void set_c_scroll(bool c_scroll) { this->c_scroll_ = c_scroll; } // continuous scrolling
  void set_c_scroll_display_length(unsigned long c_scroll_display_length) { this->c_scroll_display_length_ = c_scroll_display_length; } // display length for continuous scrolling
  void set_c_scroll_spacer(std::string c_scroll_spacer) { this->c_scroll_spacer_ = c_scroll_spacer; } // spacer between the end and the beginning of the continuous scroll
  void set_scroll_speed(unsigned long scroll_speed) { this->scroll_speed_ = scroll_speed; }
  void set_scroll_dwell(unsigned long scroll_dwell) { this->scroll_dwell_ = scroll_dwell; }
  void set_scroll_delay(unsigned long scroll_delay) { this->scroll_delay_ = scroll_delay; }
  void set_custom_buffer(unsigned long custom_buffer) { this->custom_buffer_ = custom_buffer; }  // custom buffer if needed
  void update() override;
  //// Clear display
  void set_brightness(float level);
  float get_brightness();

  /// Print the given text
  void print(const char *str);
  /// Print the given string
  void print(const std::string &str);
  /// Evaluate the printf-format and print the text
  void printf(const char *format, ...) __attribute__((format(printf, 2, 3)));

#ifdef USE_TIME
  /// Evaluate the strftime-format and print the text
  void strftime(const char *format, time::ESPTime time) __attribute__((format(strftime, 2, 0)));
#endif

 protected:
  void command_(uint8_t value);
  void call_writer() { this->writer_(*this); }
  void display_();

  std::vector<i2c::I2CDevice *> displays_ {this};
  std::function<void(HT16K33AlphaDisplay &)> writer_;
  bool scroll_ {false};
  bool c_scroll_ {false};
  unsigned long c_scroll_display_length_ {4};
  std::string c_scroll_spacer_ {"-."};
  unsigned long scroll_speed_ {250};
  unsigned long scroll_dwell_ {2000};
  unsigned long scroll_delay_ {750};
  unsigned long last_scroll_ {0};
  int custom_buffer_ {64};
  uint8_t buffer_[256];
  int buffer_fill_ {0};
  int offset_ {0};
  uint8_t brightness_ = 16;
};

}  // namespace ht16k33_alpha
}  // namespace esphome
