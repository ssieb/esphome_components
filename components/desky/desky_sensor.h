#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace desky {

class DeskySensor : public Component,  public sensor::Sensor, public uart::UARTDevice {
 public:
  float get_setup_priority() const override { return setup_priority::LATE; }
  void loop() override;
  void dump_config() override;
};

}  // namespace desky
}  // namespace esphome
