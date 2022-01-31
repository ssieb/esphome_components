#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace priva {

class PrivaComponent : public Component, public uart::UARTDevice {
 public:
  void dump_config() override;
  void loop() override;

  float get_setup_priority() const { return setup_priority::DATA; }

  void set_temperature_sensor(sensor::Sensor *temperature_sensor) { temperature_sensor_ = temperature_sensor; }
  void set_humidity_sensor(sensor::Sensor *humidity_sensor) { humidity_sensor_ = humidity_sensor; }

 protected:
  sensor::Sensor *temperature_sensor_{nullptr};
  sensor::Sensor *humidity_sensor_{nullptr};
  std::vector<uint8_t> data_;
};

}  // namespace priva
}  // namespace esphome
