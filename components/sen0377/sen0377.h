#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace sen0377 {

class Sen0377 : public PollingComponent, public i2c::I2CDevice {
 public:
  void set_co_sensor(sensor::Sensor *sensor) { this->co_sensor_ = sensor; }
  void set_methane_sensor(sensor::Sensor *sensor) { this->methane_sensor_ = sensor; }
  void set_ethanol_sensor(sensor::Sensor *sensor) { this->ethanol_sensor_ = sensor; }
  void set_hydrogen_sensor(sensor::Sensor *sensor) { this->hydrogen_sensor_ = sensor; }
  void set_ammonia_sensor(sensor::Sensor *sensor) { this->ammonia_sensor_ = sensor; }
  void set_no2_sensor(sensor::Sensor *sensor) { this->no2_sensor_ = sensor; }
  void set_co_bsensor(binary_sensor::BinarySensor *sensor) { this->co_bsensor_ = sensor; }
  void set_methane_bsensor(binary_sensor::BinarySensor *sensor) { this->methane_bsensor_ = sensor; }
  void set_ethanol_bsensor(binary_sensor::BinarySensor *sensor) { this->ethanol_bsensor_ = sensor; }
  void set_propane_bsensor(binary_sensor::BinarySensor *sensor) { this->propane_bsensor_ = sensor; }
  void set_isobutane_bsensor(binary_sensor::BinarySensor *sensor) { this->isobutane_bsensor_ = sensor; }
  void set_hydrogen_bsensor(binary_sensor::BinarySensor *sensor) { this->hydrogen_bsensor_ = sensor; }
  void set_h2s_bsensor(binary_sensor::BinarySensor *sensor) { this->h2s_bsensor_ = sensor; }
  void set_ammonia_bsensor(binary_sensor::BinarySensor *sensor) { this->ammonia_bsensor_ = sensor; }
  void set_no_bsensor(binary_sensor::BinarySensor *sensor) { this->no_bsensor_ = sensor; }
  void set_no2_bsensor(binary_sensor::BinarySensor *sensor) { this->no2_bsensor_ = sensor; }

  void setup() override;
  void loop() override;
  void update() override;

  void dump_config() override;

  float get_setup_priority() const override { return setup_priority::DATA; }

 protected:
  bool waking_{false};
  bool warming_{false};
  uint32_t start_time_;
  float init_ox_;
  float init_red_;

  sensor::Sensor *co_sensor_{nullptr};
  sensor::Sensor *methane_sensor_{nullptr};
  sensor::Sensor *ethanol_sensor_{nullptr};
  sensor::Sensor *hydrogen_sensor_{nullptr};
  sensor::Sensor *ammonia_sensor_{nullptr};
  sensor::Sensor *no2_sensor_{nullptr};
  binary_sensor::BinarySensor *co_bsensor_{nullptr};
  binary_sensor::BinarySensor *methane_bsensor_{nullptr};
  binary_sensor::BinarySensor *ethanol_bsensor_{nullptr};
  binary_sensor::BinarySensor *propane_bsensor_{nullptr};
  binary_sensor::BinarySensor *isobutane_bsensor_{nullptr};
  binary_sensor::BinarySensor *hydrogen_bsensor_{nullptr};
  binary_sensor::BinarySensor *h2s_bsensor_{nullptr};
  binary_sensor::BinarySensor *ammonia_bsensor_{nullptr};
  binary_sensor::BinarySensor *no_bsensor_{nullptr};
  binary_sensor::BinarySensor *no2_bsensor_{nullptr};
};

}  // namespace sen0377
}  // namespace esphome
