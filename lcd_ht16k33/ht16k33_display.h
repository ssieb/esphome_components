#pragma once

#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/components/i2c/i2c.h"

#ifdef USE_TIME
#include "esphome/components/time/real_time_clock.h"
#endif

namespace esphome {
namespace lcd_ht16k33 {

class HT16K33LCDDisplay : public PollingComponent, public i2c::I2CDevice {
 public:
  void set_writer(std::function<void(HT16K33LCDDisplay &)> &&writer) { this->writer_ = std::move(writer); }
  void setup() override;
  void loop() override;
  float get_setup_priority() const override;
  void set_scroll(bool scroll) { this->scroll_ = scroll; }
  void set_scroll_speed(unsigned long scroll_speed) { this->scroll_speed_ = scroll_speed; }
  void set_scroll_dwell(unsigned long scroll_dwell) { this->scroll_dwell_ = scroll_dwell; }
  void set_scroll_delay(unsigned long scroll_delay) { this->scroll_delay_ = scroll_delay; }
  void update() override;
  //// Clear LCD display
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

  std::function<void(HT16K33LCDDisplay &)> writer_;
  bool scroll_ {false};
  unsigned long scroll_speed_ {250};
  unsigned long scroll_dwell_ {2000};
  unsigned long scroll_delay_ {750};
  unsigned long last_scroll_ {0};
  uint8_t buffer_[64];
  int buffer_fill_ {0};
  int offset_ {0};
  uint8_t brightness_ = 16;
};

}  // namespace lcd_ht16k33
}  // namespace esphome
