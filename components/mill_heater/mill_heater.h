#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/core/automation.h"

namespace esphome {
namespace mill {

class Mill : public i2c::I2CDevice, public Component {
 public:
  void setup() override;
  void loop() override;

  float get_setup_priority() const override;

  void set_temp(float temp);
  void set_power(int power);

 protected:
  float temp_{0};
  int power_{0};

  void update_();
};

}  // namespace mill
}  // namespace esphome
