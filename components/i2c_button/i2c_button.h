#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/core/automation.h"

namespace esphome {
namespace i2c_button {

class I2C_Button : public i2c::I2CDevice, public Component {
 public:
  void setup() override;
  void loop() override;

  float get_setup_priority() const override;

  Trigger<> *get_a_click_trigger() const { return a_click_trigger_; }
  Trigger<> *get_a_double_click_trigger() const { return a_double_click_trigger_; }
  Trigger<> *get_a_long_click_trigger() const { return a_long_click_trigger_; }
  Trigger<> *get_b_click_trigger() const { return b_click_trigger_; }
  Trigger<> *get_b_double_click_trigger() const { return b_double_click_trigger_; }
  Trigger<> *get_b_long_click_trigger() const { return b_long_click_trigger_; }

 protected:
  Trigger<> *a_click_trigger_ = new Trigger<>();
  Trigger<> *a_double_click_trigger_ = new Trigger<>();
  Trigger<> *a_long_click_trigger_ = new Trigger<>();
  Trigger<> *b_click_trigger_ = new Trigger<>();
  Trigger<> *b_double_click_trigger_ = new Trigger<>();
  Trigger<> *b_long_click_trigger_ = new Trigger<>();
};

}  // namespace i2c_button
}  // namespace esphome
