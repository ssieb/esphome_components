#pragma once

#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/components/i2c/i2c.h"

#ifdef USE_TIME
#include "esphome/components/time/real_time_clock.h"
#include "esphome/core/time.h"
#endif

namespace esphome {
namespace ht16k33 {

// First set bit determines command, bits after that are the data.
constexpr uint8_t DISPLAY_COMMAND_SET_DDRAM_ADDR = 0x00;
constexpr uint8_t DISPLAY_COMMAND_SYSTEM_SETUP = 0x21;
constexpr uint8_t DISPLAY_COMMAND_DISPLAY_OFF = 0x80;
constexpr uint8_t DISPLAY_COMMAND_DISPLAY_ON = 0x81;
constexpr uint8_t DISPLAY_COMMAND_DIMMING = 0xE0;

class HT16K33BaseDisplay : public PollingComponent, public i2c::I2CDevice {
 public:
  void set_writer(std::function<void(HT16K33BaseDisplay &)> &&writer) { this->writer_ = std::move(writer); }
  void dump_config() override;
  void setup() override;
  void loop() override;
  float get_setup_priority() const override;
  void add_secondary_display(i2c::I2CDevice *display) { this->displays_.push_back(display); }
  void set_scroll(bool scroll) { this->scroll_ = scroll; }
  void set_scroll_speed(unsigned long scroll_speed) { this->scroll_speed_ = scroll_speed; }
  void set_scroll_dwell(unsigned long scroll_dwell) { this->scroll_dwell_ = scroll_dwell; }
  void set_scroll_delay(unsigned long scroll_delay) { this->scroll_delay_ = scroll_delay; }
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
  void strftime(const char *format, ESPTime time) __attribute__((format(strftime, 2, 0)));
#endif

 protected:
  void command_(uint8_t value);
  void call_writer() { this->writer_(*this); }

  virtual void display_() = 0;
  virtual uint16_t read_character_(uint8_t c) const = 0;
  virtual uint16_t decimal_point_mask_() const = 0;
  virtual bool supports_colon_() const =0;

  std::vector<i2c::I2CDevice *> displays_ {this};
  std::function<void(HT16K33BaseDisplay &)> writer_;
  bool scroll_ {false};
  unsigned long scroll_speed_ {250};
  unsigned long scroll_dwell_ {2000};
  unsigned long scroll_delay_ {750};
  unsigned long last_scroll_ {0};
  uint8_t buffer_[64];
  int buffer_fill_ {0};
  int offset_ {0};
  uint8_t brightness_ = 16;
  bool show_colon_ {false};
};

}  // namespace ht16k33
}  // namespace esphome
