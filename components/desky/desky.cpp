#include "desky.h"
#include "esphome/core/log.h"

namespace esphome {
namespace desky {

static const char *TAG = "desky";

const char *desky_operation_to_str(DeskyOperation op) {
  switch (op) {
    case DESKY_OPERATION_IDLE:
      return "IDLE";
    case DESKY_OPERATION_RAISING:
      return "RAISING";
    case DESKY_OPERATION_LOWERING:
      return "LOWERING";
    default:
      return "UNKNOWN";
  }
}

void Desky::setup() {
  if (this->up_pin_ != nullptr)
    this->up_pin_->digital_write(false);
  if (this->down_pin_ != nullptr)
    this->down_pin_->digital_write(false);
  if (this->request_pin_ != nullptr) {
    this->request_pin_->digital_write(true);
    this->request_time_ = millis();
  }
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
    if (abs(this->target_pos_ - this->current_pos_) < this->stopping_distance_)
      this->stop();
    if ((this->timeout_ >= 0) && (millis() - this->start_time_ >= this->timeout_))
      this->stop();
  }

  if ((this->request_time_ > 0) && (millis() - this->request_time_ >= 100)) {
    this->request_pin_->digital_write(false);
    this->request_time_ = 0;
  }
}

void Desky::dump_config() {
  ESP_LOGCONFIG(TAG, "Desky desk:");
  LOG_SENSOR("", "Height", this->height_sensor_);
  LOG_PIN("Up pin: ", this->up_pin_);
  LOG_PIN("Down pin: ", this->down_pin_);
  LOG_PIN("Request pin: ", this->request_pin_);
}

void Desky::move_to(int target_pos) {
  if (abs(target_pos - this->current_pos_) < this->stopping_distance_)
    return;
  if (target_pos > this->current_pos_) {
    if (this->up_pin_ == nullptr)
      return;
    this->up_pin_->digital_write(true);
    this->current_operation = DESKY_OPERATION_RAISING;
  } else {
    if (this->down_pin_ == nullptr)
      return;
    this->down_pin_->digital_write(true);
    this->current_operation = DESKY_OPERATION_LOWERING;
  }
  this->target_pos_ = target_pos;
  if (this->timeout_ >= 0)
    this->start_time_ = millis();
}

void Desky::stop() {
  this->target_pos_ = -1;
  if (this->up_pin_ != nullptr)
    this->up_pin_->digital_write(false);
  if (this->down_pin_ != nullptr)
    this->down_pin_->digital_write(false);
  this->current_operation = DESKY_OPERATION_IDLE;
}

}  // namespace desky
}  // namespace esphome
