#include "shift_register.h"
#include "esphome/core/log.h"

namespace esphome {
namespace shift_register {

static const char *TAG = "shift_register";

void ShiftRegister::setup() {
  ESP_LOGCONFIG(TAG, "Setting up shift register...");

  this->outputs_ = new uint8_t[this->count_];
  this->clock_->setup();
  this->clock_->digital_write(false);
  this->data_->setup();
  this->data_->digital_write(false);
  if (this->latch_ != nullptr) {
    this->latch_->setup();
    this->latch_->digital_write(false);
  }
}

bool ShiftRegister::digital_read(uint8_t pin) {
  uint8_t chip = pin / 8;
  uint8_t bit = 1 << (pin % 8);
  return ((this->outputs_[chip] & bit) > 0);
}

void ShiftRegister::digital_write(uint8_t pin, bool value) {
  uint8_t chip = pin / 8;
  uint8_t bit = 1 << (pin % 8);
  if (value)
    this->outputs_[chip] |= bit;
  else
    this->outputs_[chip] &= bit;
  this->update_();
}

void ShiftRegister::update_() {
  for (int chip = 0; chip < this->count_; chip++) {
    uint8_t data = this->outputs_[chip];
    for (int pin = 7; pin >= 0; pin--) {
      this->data_->digital_write((data & (1 << pin)) > 0);
      this->clock_->digital_write(true);
      this->clock_->digital_write(false);
    }
  }
  if (this->latch_ != nullptr) {
    this->latch_->digital_write(true);
    this->latch_->digital_write(false);
  }
}


ShiftRegisterGPIOPin::ShiftRegisterGPIOPin(ShiftRegister *parent, uint8_t pin, bool inverted)
    : GPIOPin(pin, OUTPUT, inverted), parent_(parent) {}
void ShiftRegisterGPIOPin::setup() { this->pin_mode(this->mode_); }
void ShiftRegisterGPIOPin::pin_mode(uint8_t mode) {}
bool ShiftRegisterGPIOPin::digital_read() { return this->parent_->digital_read(this->pin_) != this->inverted_; }
void ShiftRegisterGPIOPin::digital_write(bool value) { this->parent_->digital_write(this->pin_, value != this->inverted_); }

}  // namespace shift_register
}  // namespace esphome
