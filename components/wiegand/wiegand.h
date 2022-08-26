#pragma once

#include "esphome/core/automation.h"
#include "esphome/core/component.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace wiegand {

struct WiegandStore {
  ISRInternalGPIOPin d0;
  ISRInternalGPIOPin d1;
  volatile uint64_t value{0};
  volatile uint32_t last_bit_time{0};
  volatile bool done{true};
  volatile uint8_t count{0};

  static void d0_gpio_intr(WiegandStore *arg);
  static void d1_gpio_intr(WiegandStore *arg);
};

class WiegandStrTrigger : public Trigger<std::string> {
};

class WiegandDigitTrigger : public Trigger<uint8_t> {
};

class Wiegand : public Component {
 public:
  float get_setup_priority() const override { return setup_priority::HARDWARE; }
  void setup() override;
  void loop() override;
  void dump_config() override;

  void set_d0_pin(InternalGPIOPin *pin) { this->d0_pin_ = pin; };
  void set_d1_pin(InternalGPIOPin *pin) { this->d1_pin_ = pin; };
  void register_tag_trigger(WiegandStrTrigger *trig) { this->tag_triggers_.push_back(trig); }
  void register_digit_trigger(WiegandDigitTrigger *trig) { this->digit_triggers_.push_back(trig); }
  void register_pin_trigger(WiegandStrTrigger *trig) { this->pin_triggers_.push_back(trig); }

 protected:
  InternalGPIOPin *d0_pin_;
  InternalGPIOPin *d1_pin_;
  WiegandStore store_{};
  std::vector<WiegandStrTrigger *> tag_triggers_;
  std::vector<WiegandDigitTrigger *> digit_triggers_;
  std::vector<WiegandStrTrigger *> pin_triggers_;
};

}  // namespace wiegand
}  // namespace esphome
