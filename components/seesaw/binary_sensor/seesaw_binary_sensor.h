#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "../seesaw.h"

namespace esphome {
namespace seesaw {

class SeesawBinarySensor : public binary_sensor::BinarySensor, public Component {
 public:
  void setup() override;
  void dump_config() override;
  void loop() override;
  void set_parent(Seesaw *parent) { this->parent_ = parent; }
  void set_pin(int pin) { this->pin_ = pin; }

 protected:
  Seesaw *parent_;
  int pin_;
};

}  // namespace seesaw
}  // namespace esphome
