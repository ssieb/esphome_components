#pragma once

#include "esphome/core/component.h"
#include "esphome/components/light/light_output.h"
#include "../seesaw.h"

namespace esphome {
namespace seesaw {

class SeesawNeopixel : public light::LightOutput, public Component {
 public:
  void setup() override;
  void set_parent(Seesaw *parent) { parent_ = parent; }
  light::LightTraits get_traits() override;
  void write_state(light::LightState *state) override;
 protected:
  Seesaw *parent_;
};

}  // namespace seesaw
}  // namespace esphome
