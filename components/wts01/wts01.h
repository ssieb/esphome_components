#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace wts01 {

class WTS01 : public sensor::Sensor, public Component, public uart::UARTDevice {
 public:
  void dump_config() override;
  void loop() override;

  float get_setup_priority() const { return setup_priority::DATA; }

 protected:
  bool reading_ = false;
  std::vector<uint8_t> data_;
  uint32_t last_transmission_{0};
};

}  // namespace wts01
}  // namespace esphome
