#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace t67xx {

class T67xx : public sensor::Sensor, public PollingComponent, public i2c::I2CDevice {
 public:
  void update() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void start_calibration();
};

}  // namespace t67xx
}  // namespace esphome
