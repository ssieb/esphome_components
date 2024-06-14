#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace axp2101 {

class AXP2101 : public PollingComponent, public i2c::I2CDevice {
 public:
  void set_battery_level(sensor::Sensor *battery_level) { this->battery_level_ = battery_level; }

  void update() override;
  void setup() override;
  void dump_config() override;

  float get_setup_priority() const override;

 protected:
  sensor::Sensor *battery_level_{nullptr};
};

}  // namespace axp2101
}  // namespace esphome
