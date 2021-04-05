#include "pca9536d.h"
#include "esphome/core/log.h"

namespace esphome {
namespace pca9536d {

static const char *TAG = "pca9536d";

void PCA9536DComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up PCA9536D...");
  if (!read_gpio_()) {
    ESP_LOGE(TAG, "PCA9536D not available under 0x%02X", address_);
    mark_failed();
    return;
  }

  write_gpio_();
}

void PCA9536DComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "PCA9536D:");
  LOG_I2C_DEVICE(this)
  if (is_failed())
    ESP_LOGE(TAG, "Communication with PCA9536D failed!");
}

bool PCA9536DComponent::digital_read(uint8_t pin) {
  read_gpio_();
  return inputs_ & (1 << pin);
}

void PCA9536DComponent::digital_write(uint8_t pin, bool value) {
  if (value)
    outputs_ |= (1 << pin);
  else
    outputs_ &= ~(1 << pin);
  write_gpio_();
}

void PCA9536DComponent::pin_mode(uint8_t pin, uint8_t mode) {
  switch (mode) {
    case PCA9536D_INPUT:
      modes_ |= 1 << pin;
      break;
    case PCA9536D_OUTPUT:
      modes_ &= ~(1 << pin);
      break;
    default:
      break;
  }
  set_modes_();
}

bool PCA9536DComponent::read_gpio_() {
  if (is_failed())
    return false;

  uint8_t data;
  if (!read_bytes(0, &data, 1)) {
    status_set_warning();
    return false;
  }

  inputs_ = data;
  status_clear_warning();
  return true;
}

bool PCA9536DComponent::write_gpio_() {
  if (is_failed())
    return false;

  uint8_t data = outputs_;
  if (!this->write_bytes(1, &data, 1)) {
    this->status_set_warning();
    return false;
  }

  this->status_clear_warning();
  return true;
}

bool PCA9536DComponent::set_modes_() {
  if (is_failed())
    return false;

  uint8_t data = modes_;
  if (!this->write_bytes(3, &data, 1)) {
    this->status_set_warning();
    return false;
  }

  this->status_clear_warning();
  return true;
}

void PCA9536DGPIOPin::setup() { pin_mode(mode_); }

bool PCA9536DGPIOPin::digital_read() { return parent_->digital_read(pin_) != inverted_; }

void PCA9536DGPIOPin::digital_write(bool value) { parent_->digital_write(pin_, value != inverted_); }

void PCA9536DGPIOPin::pin_mode(uint8_t mode) { parent_->pin_mode(pin_, mode); }

PCA9536DGPIOPin::PCA9536DGPIOPin(PCA9536DComponent *parent, uint8_t pin, uint8_t mode, bool inverted)
    : GPIOPin(pin, mode, inverted), parent_(parent) {}

}  // namespace pca9536d
}  // namespace esphome
