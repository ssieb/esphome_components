#pragma once

#include "esphome/core/component.h"
#include "esphome/components/light/light_output.h"
#include "../m5_8encoder.h"

namespace esphome {
namespace m5_8encoder {

class M5_8encoderLED : public light::LightOutput, public Component {
 public:
  void set_parent(M5_8encoder *parent) { this->parent_ = parent; }
  void set_number(uint8_t number) { this->number_ = number; }
  light::LightTraits get_traits() override;
  void write_state(light::LightState *state) override;
 protected:
  M5_8encoder *parent_;
  uint8_t number_;
};

}  // namespace m5_8encoder
}  // namespace esphome
