#include "pca963x_output.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace pca963x {

static const char *TAG = "pca963x";

static const uint8_t PCA963X_MODE1_SLEEP = 0x10;

static const uint8_t PCA963X_MODE2_BLINK = 0x20;
static const uint8_t PCA963X_MODE2_INVERTED = 0x10;
static const uint8_t PCA963X_MODE2_OUTPUT_ONACK = 0x08;
static const uint8_t PCA963X_MODE2_OUTPUT_TOTEM_POLE = 0x04;
static const uint8_t PCA963X_MODE2_OUTNE_DEFAULT = 0x01;

static const uint8_t PCA963X_REGISTER_MODE1 = 0x00;
static const uint8_t PCA963X_REGISTER_MODE2 = 0x01;
static const uint8_t PCA963X_REGISTER_LED0 = 0x02;

void PCA963XOutput::setup() {
  ESP_LOGCONFIG(TAG, "Setting up PCA963xOutput Component...");

  // clear/ reset registers
  this->write_byte(PCA963X_REGISTER_MODE1, 0x0);
  this->write_byte(PCA963X_REGISTER_MODE2, 0x0);
}

void PCA963XOutput::dump_config() {
  ESP_LOGCONFIG(TAG, "PCA963X:");
  ESP_LOGCONFIG(TAG, "  Mode: 0x%02X", this->mode_);
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Setting up PCA963X failed!");
  }
}

void PCA963XOutput::wakeUp() {

    uint8_t prevReg = this->read_byte(PCA963X_REGISTER_MODE1);
    uint8_t newReg = prevReg & ~(PCA963X_MODE1_SLEEP);

    this->write_byte(PCA963X_REGISTER_MODE1, newReg);
}

void PCA963XOutput::sleep() {

    uint8_t prevReg = this->read_byte(PCA963X_REGISTER_MODE1);
    uint8_t newReg = prevReg | (PCA963X_MODE1_SLEEP);

    this->write_byte(PCA963X_REGISTER_MODE1, newReg);
}

void PCA963XOutput::loop() {
  if (!this->update_)
    return;

  uint8_t prevReg = this->read_byte(PCA963X_REGISTER_MODE2);
  uint8_t newReg = prevReg & ~(PCA963X_MODE2_BLINK |
                              PCA963X_MODE2_INVERTED |
                              PCA963X_MODE2_OUTPUT_TOTEM_POLE);
  if (this->blink_mode_) newReg = newReg | PCA963X_MODE2_BLINK;
  if (this->blink_mode_) newReg = newReg | PCA963X_MODE2_OUTPUT_TOTEM_POLE;
  if (this->blink_mode_) newReg = newReg | PCA963X_MODE2_INVERTED;
  this->write_byte(PCA963X_REGISTER_MODE2, newReg);

  for (uint8_t channel = 0; channel < this->max_channels_; channel++) {
    uint8_t reg = PCA963X_REGISTER_LED0 + channel;
    if (!this->write_byte(reg, pwm_values_[channel])) {
      this->status_set_warning();
      return;
    }
  }

  this->status_clear_warning();
  this->update_ = false;
}

PCA963XChannel *PCA963XOutput::create_channel(uint8_t channel) {
  if (channel >= this->max_channels_) {
    ESP_LOGE(TAG, "You cant use channel: %d !", channel);
    this->mark_failed();
    return;
  }
  return new PCA963XChannel(this, channel);
}

void PCA963XChannel::write_state(float state) {
  uint8_t value = static_cast<uint8_t>(roundf(state * 255));
  this->parent_->set_channel_value_(this->channel_, value);
}

}  // namespace pca963x
}  // namespace esphome
