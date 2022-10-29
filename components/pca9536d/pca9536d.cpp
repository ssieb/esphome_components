#include "pca9536d.h"
#include "esphome/core/log.h"

namespace esphome {
namespace pca9536d {

static const char *TAG = "pca9536d";

void PCA9536D::setup() {
  ESP_LOGCONFIG(TAG, "Setting up PCA9536D...");
  if (!this->read_gpio_()) {
    ESP_LOGE(TAG, "PCA9536D not available under 0x%02X", this->address_);
    this->mark_failed();
    return;
  }

  this->write_gpio_();
}

void PCA9536D::dump_config() {
  ESP_LOGCONFIG(TAG, "PCA9536D:");
  LOG_I2C_DEVICE(this)
  if (this->is_failed())
    ESP_LOGE(TAG, "Communication with PCA9536D failed!");
}

bool PCA9536D::digital_read(uint8_t pin) {
  this->read_gpio_();
  return this->inputs_ & (1 << pin);
}

void PCA9536D::digital_write(uint8_t pin, bool value) {
  if (value)
    this->outputs_ |= (1 << pin);
  else
    this->outputs_ &= ~(1 << pin);
  this->write_gpio_();
}

void PCA9536D::set_pin_mode(uint8_t pin, uint8_t mode) {
  switch (mode) {
    case gpio::FLAG_INPUT:
      this->modes_ |= 1 << pin;
      break;
    case gpio::FLAG_OUTPUT:
      this->modes_ &= ~(1 << pin);
      break;
  }
  this->set_modes_();
}

bool PCA9536D::read_gpio_() {
  if (this->is_failed())
    return false;

  uint8_t data;
  if (!this->read_bytes(0, &data, 1)) {
    this->status_set_warning();
    return false;
  }

  this->inputs_ = data;
  this->status_clear_warning();
  return true;
}

bool PCA9536D::write_gpio_() {
  if (this->is_failed())
    return false;

  uint8_t data = this->outputs_;
  if (!this->write_bytes(1, &data, 1)) {
    this->status_set_warning();
    return false;
  }

  this->status_clear_warning();
  return true;
}

bool PCA9536D::set_modes_() {
  if (this->is_failed())
    return false;

  uint8_t data = this->modes_;
  if (!this->write_bytes(3, &data, 1)) {
    this->status_set_warning();
    return false;
  }

  this->status_clear_warning();
  return true;
}

void PCA9536DGPIOPin::setup() { this->pin_mode(this->flags_); }

bool PCA9536DGPIOPin::digital_read() { return this->parent_->digital_read(this->pin_) != this->inverted_; }

void PCA9536DGPIOPin::digital_write(bool value) { this->parent_->digital_write(this->pin_, value != this->inverted_); }

void PCA9536DGPIOPin::pin_mode(gpio::Flags flags) { this->parent_->set_pin_mode(this->pin_, flags); }

std::string PCA9536DGPIOPin::dump_summary() const {
  return str_sprintf("%u via PCA9536D", pin_);
}

}  // namespace pca9536d
}  // namespace esphome
