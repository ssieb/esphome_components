#include "desky.h"
#include "esphome/core/log.h"

namespace esphome {
namespace desky {

static const char *TAG = "desky";

void Desky::setup() {
  if (this->up_pin_ != nullptr)
    this->up_pin_->digital_write(false);
  if (this->down_pin_ != nullptr)
    this->down_pin_->digital_write(false);
}

void Desky::loop() {
  static int state = 0;
  static uint8_t high_byte;

  while (this->available()) {
    uint8_t c;
    int value;
    this->read_byte(&c);
    switch (state) {
     case 0:
      if (c == 1)
	state = 1;
      break;
     case 1:
      if (c == 1)
	state = 2;
      else
	state = 0;
      break;
     case 2:
      high_byte = c;
      state = 3;
      break;
     case 3:
      value = (high_byte << 8) + c;
      this->current_pos_ = value;
      if (this->height_sensor_ != nullptr)
        this->height_sensor_->publish_state(value);
      state = 0;
      break;
    }
  }

  if (this->target_pos_ >= 0) {
    if (abs(this->target_pos_ - this->current_pos_) < this->stopping_distance_) {
      this->target_pos_ = -1;
      if (this->up_pin_ != nullptr)
        this->up_pin_->digital_write(false);
      if (this->down_pin_ != nullptr)
        this->down_pin_->digital_write(false);
    }
  }
}

void Desky::dump_config() {
  ESP_LOGCONFIG(TAG, "Desky desk:");
  LOG_SENSOR("", "Height", this->height_sensor_);
  LOG_PIN("Up pin: ", this->up_pin_);
  LOG_PIN("Down pin: ", this->down_pin_);
}

void Desky::move_to(int target_pos) {
  if (abs(target_pos - this->current_pos_) < this->stopping_distance_)
    return;
  if (target_pos > this->current_pos_) {
    if (this->up_pin_ == nullptr)
      return;
    this->up_pin_->digital_write(true);
  } else {
    if (this->down_pin_ == nullptr)
      return;
    this->down_pin_->digital_write(true);
  }
  this->target_pos_ = target_pos;
}

}  // namespace desky
}  // namespace esphome
