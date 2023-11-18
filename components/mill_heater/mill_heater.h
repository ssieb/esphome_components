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
  void set_plus_key(binary_sensor::BinarySensor *bsensor) { this->plus_key_ = bsensor; }
  void set_minus_key(binary_sensor::BinarySensor *bsensor) { this->minus_key_ = bsensor; }
  void set_wifi_key(binary_sensor::BinarySensor *bsensor) { this->wifi_key_ = bsensor; }
  void set_clock_key(binary_sensor::BinarySensor *bsensor) { this->clock_key_ = bsensor; }

  void set_power(int power);
  void set_dark(bool dark) { this->dark_ = dark; }

 protected:
  climate::Climate *climate_;
  binary_sensor::BinarySensor *plus_key_{nullptr};
  binary_sensor::BinarySensor *minus_key_{nullptr};
  binary_sensor::BinarySensor *wifi_key_{nullptr};
  binary_sensor::BinarySensor *clock_key_{nullptr};

  bool dark_{false};
  bool dark_override_{false};
  uint32_t wake_state_;
  float temp_{NAN};
  int power_{0};
  bool power_led_{false};
  bool lightning_led_{false};
  bool wifi_led_{false};

  void update_();
  void update_climate_state_(climate::Climate &clim);
};

}  // namespace mill
}  // namespace esphome
