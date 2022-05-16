#include "pca9634_output.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace pca9634 {

static const char *TAG = "pca9634";

static const uint8_t PCA9634_ADDR_SOFTWARE_RESET = 0x06;

static const uint8_t PCA9634_MODE2_BLINK = 0x20;
static const uint8_t PCA9634_MODE2_INVERTED = 0x10;
static const uint8_t PCA9634_MODE2_OUTPUT_ONACK = 0x08;
static const uint8_t PCA9634_MODE2_OUTPUT_TOTEM_POLE = 0x04;
static const uint8_t PCA9634_MODE2_OUTNE_DEFAULT = 0x01;

static const uint8_t PCA9634_REGISTER_MODE1 = 0x00;
static const uint8_t PCA9634_REGISTER_MODE2 = 0x01;
static const uint8_t PCA9634_REGISTER_LED0 = 0x02;
static const uint8_t PCA9634_REGISTER_PRE_SCALE = 0xFE;


void PCA9634Output::setup() {
  ESP_LOGCONFIG(TAG, "Setting up PCA9634OutputComponent...");

  uint8_t data[14];
  ESP_LOGV(TAG, "  Resetting device...");
  data[0] = 0xa5; data[1] = 0x5a;
  if (!this->write_bytes_raw(PCA9634_ADDR_SOFTWARE_RESET, data, 2)) {
    this->mark_failed();
    return;
  }

  data[0] = 0; // MODE1
  data[1] = this->group_mode_ | this->output_mode_ | PCA9634_MODE2_OUTNE_DEFAULT; // MODE2
  if (this->inverted_)
    data[1] |= PCA9634_MODE2_INVERTED;

  if (!this->write_byte(PCA9685_REGISTER_MODE1, PCA9685_MODE1_RESTART | PCA9685_MODE1_AUTOINC)) {
    this->mark_failed();
    return;
  }
  if (!this->write_byte(PCA9685_REGISTER_MODE2, this->mode_)) {
    this->mark_failed();
    return;
  }

  int pre_scaler = static_cast<int>((25000000 / (4096 * this->frequency_)) - 1);
  if (pre_scaler > 255)
    pre_scaler = 255;
  if (pre_scaler < 3)
    pre_scaler = 3;

  ESP_LOGV(TAG, "  -> Prescaler: %d", pre_scaler);

  uint8_t mode1;
  if (!this->read_byte(PCA9685_REGISTER_MODE1, &mode1)) {
    this->mark_failed();
    return;
  }
  mode1 = (mode1 & ~PCA9685_MODE1_RESTART) | PCA9685_MODE1_SLEEP;
  if (!this->write_byte(PCA9685_REGISTER_MODE1, mode1)) {
    this->mark_failed();
    return;
  }
  if (!this->write_byte(PCA9685_REGISTER_PRE_SCALE, pre_scaler)) {
    this->mark_failed();
    return;
  }

  mode1 = (mode1 & ~PCA9685_MODE1_SLEEP) | PCA9685_MODE1_RESTART;
  if (!this->write_byte(PCA9685_REGISTER_MODE1, mode1)) {
    this->mark_failed();
    return;
  }
  delayMicroseconds(500);

  this->loop();
}

void PCA9634Output::dump_config() {
  ESP_LOGCONFIG(TAG, "PCA9634:");
  ESP_LOGCONFIG(TAG, "  Mode: 0x%02X", this->mode_);
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Setting up PCA9634 failed!");
  }
}

void PCA9634Output::loop() {
  if (!this->update_)
    return;

  const uint16_t num_channels = this->max_channel_ - this->min_channel_ + 1;
  for (uint8_t channel = this->min_channel_; channel <= this->max_channel_; channel++) {
    uint16_t phase_begin = uint16_t(channel - this->min_channel_) / num_channels * 4096;
    uint16_t phase_end;
    uint16_t amount = this->pwm_amounts_[channel];
    if (amount == 0) {
      phase_end = 4096;
    } else if (amount >= 4096) {
      phase_begin = 4096;
      phase_end = 0;
    } else {
      phase_end = phase_begin + amount;
      if (phase_end >= 4096)
        phase_end -= 4096;
    }

    ESP_LOGVV(TAG, "Channel %02u: amount=%04u phase_begin=%04u phase_end=%04u", channel, amount, phase_begin,
              phase_end);

    uint8_t data[4];
    data[0] = phase_begin & 0xFF;
    data[1] = (phase_begin >> 8) & 0xFF;
    data[2] = phase_end & 0xFF;
    data[3] = (phase_end >> 8) & 0xFF;

    uint8_t reg = PCA9634_REGISTER_LED0 + 4 * channel;
    if (!this->write_bytes(reg, data, 4)) {
      this->status_set_warning();
      return;
    }
  }

  this->status_clear_warning();
  this->update_ = false;
}

PCA9634Channel *PCA9634Output::create_channel(uint8_t channel) {
  return new PCA9634Channel(this, channel);
}

void PCA9634Channel::write_state(float state) {
  uint8_t value = static_cast<uint8_t>(roundf(state * 255));
  this->parent_->set_channel_value_(this->channel_, value);
}

}  // namespace pca9634
}  // namespace esphome
