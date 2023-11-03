#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace mill {

class Mill : public i2c::I2CDevice, public Component {
 public:
  void setup() override;
  void loop() override;

  float get_setup_priority() const override;

  void set_plus_key(binary_sensor::BinarySensor *bsensor) { this->plus_key_ = bsensor; }
  void set_minus_key(binary_sensor::BinarySensor *bsensor) { this->minus_key_ = bsensor; }
  void set_wifi_key(binary_sensor::BinarySensor *bsensor) { this->wifi_key_ = bsensor; }
  void set_clock_key(binary_sensor::BinarySensor *bsensor) { this->clock_key_ = bsensor; }

  void set_temp(float temp);
  void set_power(int power);
  void set_button(int bnum);

 protected:
  binary_sensor::BinarySensor *plus_key_{nullptr};
  binary_sensor::BinarySensor *minus_key_{nullptr};
  binary_sensor::BinarySensor *wifi_key_{nullptr};
  binary_sensor::BinarySensor *clock_key_{nullptr};

  float temp_{0};
  int power_{0};
  int bnum_{0};

  void update_();
};

}  // namespace mill
}  // namespace esphome
