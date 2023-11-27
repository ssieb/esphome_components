#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/climate/climate.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace mill {

class Mill : public i2c::I2CDevice, public Component {
 public:
  void setup() override;
  void loop() override;

  float get_setup_priority() const override;

  void set_climate(climate::Climate *clim) { this->climate_ = clim; }
  void set_dark_timeout(uint32_t timeout) { this->dark_timeout_ = timeout; }
  void set_key_repeat_delay(uint32_t delay) { this->key_repeat_delay_ = delay; }
  void set_key_repeat_interval(uint32_t interval) { this->key_repeat_interval_ = interval; }
  void set_set_temp_delay(uint32_t delay) { this->set_temp_delay_ = delay; }
  void set_wifi_key(binary_sensor::BinarySensor *bsensor) { this->wifi_key_ = bsensor; }
  void set_clock_key(binary_sensor::BinarySensor *bsensor) { this->clock_key_ = bsensor; }

  void set_power(int power);
  void set_dark(bool dark) { this->dark_ = dark; }
  bool get_dark() { return this->dark_; }
  void set_wifi(bool ison) { this->wifi_led_ = ison; }

 protected:
  climate::Climate *climate_;
  binary_sensor::BinarySensor *wifi_key_{nullptr};
  binary_sensor::BinarySensor *clock_key_{nullptr};

  bool dark_{false};
  bool dark_override_{false};
  uint32_t dark_timeout_;
  uint32_t wake_start_;
  float temp_{NAN};
  float target_temp_{NAN};
  uint8_t current_key_{0};
  uint32_t key_start_{0};
  uint32_t key_release_{0};
  uint32_t key_last_repeat_{0};
  uint32_t key_repeat_delay_;
  uint32_t key_repeat_interval_;
  uint32_t set_temp_delay_;
  bool blinking_{false};
  bool blink_on_{false};
  uint32_t blink_interval_;
  uint32_t last_blink_;
  int power_{0};
  bool power_led_{false};
  bool lightning_led_{false};
  bool wifi_led_{false};

  void update_();
  void update_climate_state_(climate::Climate &clim);
  void adjust_target_temp_(uint8_t key);
};

}  // namespace mill
}  // namespace esphome
