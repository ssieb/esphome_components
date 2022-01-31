#include "priva.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace priva {

static const char *TAG = "priva";

void PrivaComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Priva:");
  LOG_SENSOR("  ", "Temperature", temperature_sensor_);
  LOG_SENSOR("  ", "Humidity", humidity_sensor_);
  this->check_uart_settings(9600);
}

void PrivaComponent::loop() {
  static int state = 0;

  while (available()) {
    uint8_t c;
    read_byte(&c);
    if (state == 0) {
      if (c == 'T')
        state = 1;
      else if (c == 'H')
        state = 2;
      else
        continue;
      data_.clear();
      continue;
    }
    if (data_.size() > 8) {
      state = 0;
      continue;
    }
    if (c == ';') {
      data_.push_back(0);
      auto value = parse_number<float>((char *)data_.data());
      if (value.has_value()) {
        if (state == 1) {
          if (temperature_sensor_ != nullptr)
            temperature_sensor_->publish_state(value.value());
        } else if (state == 2) {
          if (humidity_sensor_ != nullptr)
            humidity_sensor_->publish_state(value.value());
        }
      }
      state = 0;
      continue;
    }
    data_.push_back(c);
  }
}

}  // namespace priva
}  // namespace esphome
