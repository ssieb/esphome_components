#pragma once

#include "esphome/core/component.h"
#include "esphome/core/esphal.h"

namespace esphome {
namespace shift_register {

class ShiftRegister : public Component {
 public:
  void setup() override;
  float get_setup_priority() const override { return setup_priority::IO; };
  void set_count(int count) { this->count_ = count; };
  void set_clock_pin(GPIOPin *clock) { this->clock_ = clock; };
  void set_data_pin(GPIOPin *data) { this->data_ = data; };
  void set_latch_pin(GPIOPin *latch) { this->latch_ = latch; };

  bool digital_read(uint8_t pin);
  void digital_write(uint8_t pin, bool value);

 protected:
  void update_();

  int count_ = 1;
  GPIOPin *clock_ = nullptr;
  GPIOPin *data_ = nullptr;
  GPIOPin *latch_ = nullptr;
  uint8_t *outputs_;

};

class ShiftRegisterGPIOPin : public GPIOPin {
 public:
  ShiftRegisterGPIOPin(ShiftRegister *parent, uint8_t pin, bool inverted = false);

  void setup() override;
  void pin_mode(uint8_t mode) override;
  bool digital_read() override;
  void digital_write(bool value) override;

 protected:
  ShiftRegister *parent_;
};

}  // namespace shift_register
}  // namespace esphome

