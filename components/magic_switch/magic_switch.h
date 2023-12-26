#pragma once

#include "esphome/core/automation.h"
#include "esphome/core/component.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace magic_switch {

class MagicSwitch : public Component {
 public:
  void setup() override;
  void loop() override;
  float get_setup_priority() const override;
  void dump_config() override;

  void set_pin(InternalGPIOPin *pin) { this->pin_ = pin; }
  void set_timeout(uint32_t timeout) { this->timeout_ = timeout; }
  Trigger<> *get_switch_trigger() const { return switch_trigger_; }

 protected:
  InternalGPIOPin *pin_{nullptr};
  uint32_t timeout_;
  Trigger<> *switch_trigger_ = new Trigger<>();

  static void edge_intr(MagicSwitch *comp);

  volatile uint32_t pulse_{0};

  ISRInternalGPIOPin isr_pin_;
  uint32_t pulse_start_{0};
};

}  // namespace magic_switch
}  // namespace esphome
