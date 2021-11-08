#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "../seesaw.h"

namespace esphome {
namespace seesaw {

class SeesawRotaryEncoder : public sensor::Sensor, public Component {
 public:
  void setup() override;
  void loop() override;

  void set_parent(Seesaw *parent) { parent_ = parent; }

 protected:
  Seesaw *parent_;
  int32_t value_{0};
};

}  // namespace seesaw
}  // namespace esphome
