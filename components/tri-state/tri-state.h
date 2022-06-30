#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/switch/switch.h"

namespace esphome {
namespace tri_state {

class TriStateSwitch : public switch_::Switch, public Component {
 public:
  void set_pin(GPIOPin *pin) { this->pin_ = pin; }
  void set_on_state(bool state) { this->on_state_ = state; }

  float get_setup_priority() const override;

  void setup() override;
  void dump_config() override;

 protected:
  void write_state(bool state) override;

  GPIOPin *pin_;
  bool on_state_;
};

}  // namespace tri_state
}  // namespace esphome
