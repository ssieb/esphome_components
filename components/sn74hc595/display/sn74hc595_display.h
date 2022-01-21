#pragma once

#include "esphome/core/component.h"
#include "../sn74hc595.h"

#ifdef USE_TIME
#include "esphome/components/time/real_time_clock.h"
#endif

namespace esphome {
namespace sn74hc595 {

class SN74HC595Display;

using sn74hc595_writer_t = std::function<void(SN74HC595Display &)>;

class SN74HC595Display : public PollingComponent {
 public:
  void set_writer(sn74hc595_writer_t &&writer) { this->writer_ = writer; }
  void set_parent(SN74HC595Component *parent) { this->parent_ = parent; }

  void setup() override;
  void loop() override;

  void dump_config() override;

  float get_setup_priority() const override;

  void update() override;

  /// Evaluate the printf-format and print the result at the given position.
  uint8_t printf(uint8_t pos, const char *format, ...) __attribute__((format(printf, 3, 4)));
  /// Evaluate the printf-format and print the result at position 0.
  uint8_t printf(const char *format, ...) __attribute__((format(printf, 2, 3)));

  /// Print `str` at the given position.
  uint8_t print(uint8_t pos, const char *str);
  /// Print `str` at position 0.
  uint8_t print(const char *str);

  void display();

#ifdef USE_TIME
  /// Evaluate the strftime-format and print the result at the given position.
  uint8_t strftime(uint8_t pos, const char *format, time::ESPTime time) __attribute__((format(strftime, 3, 0)));

  /// Evaluate the strftime-format and print the result at position 0.
  uint8_t strftime(const char *format, time::ESPTime time) __attribute__((format(strftime, 2, 0)));
#endif

 protected:
  SN74HC595Component *parent_;
  optional<sn74hc595_writer_t> writer_{};
  uint8_t buffer_[4] = {0};
};

}  // namespace sn74hc595
}  // namespace esphome
