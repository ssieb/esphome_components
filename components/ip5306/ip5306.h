#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace ip5306 {

class IP5306 : public i2c::I2CDevice, public Component {
 public:
  void setup() override;
  void loop() override;

  float get_setup_priority() const override;

  void set_battery_level(sensor::Sensor *sensor) { this->battery_level_ = sensor; }
  void set_charger_connected(binary_sensor::BinarySensor *sensor) { this->charger_connected_ = sensor; }
  void set_charge_full(binary_sensor::BinarySensor *sensor) { this->charge_full_ = sensor; }

 protected:
  sensor::Sensor *battery_level_{nullptr};
  binary_sensor::BinarySensor *charger_connected_{nullptr};
  binary_sensor::BinarySensor *charge_full_{nullptr};
};

}  // namespace ip5306
}  // namespace esphome

