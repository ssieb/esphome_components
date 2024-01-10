#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace serial {

class SerialCSV : public Component,  public uart::UARTDevice {
 public:
  float get_setup_priority() const override { return setup_priority::DATA; }
  void loop() override;
  void dump_config() override;

  void add_sensor(int index, sensor::Sensor *sens) {
    this->sensors_.push_back(std::make_pair(index, sens));
  }

 protected:
  void parse_values_();
  std::vector<uint8_t> rx_message_;
  std::vector<std::pair<int, sensor::Sensor *>> sensors_;
};

}  // namespace serial
}  // namespace esphome
