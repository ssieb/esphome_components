#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "../seesaw.h"

namespace esphome {
namespace seesaw {

class SeesawTouch : public sensor::Sensor, public PollingComponent {
 public:
  void setup() override;
  void update() override;

  void set_parent(Seesaw *parent) { parent_ = parent; }
  void set_pin(uint8_t pin) { pin_ = pin; }

 protected:
  Seesaw *parent_;
  uint8_t pin_{0};
};

}  // namespace seesaw
}  // namespace esphome
