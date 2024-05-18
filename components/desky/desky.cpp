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
  static uint8_t protocol = 0;

  while (this->available()) {
    uint8_t c;
    float value;
    this->read_byte(&c);
    switch (state) {
     case 0:
      if ((c == 1) || (c == 242)) {
	state = 1;
	protocol = c;
      }
      break;
     case 1:
      if (c == protocol)
	state = 2;
      else
	state = 0;
      this->rx_data_.clear();
      break;
     case 2:
      this->rx_data_.push_back(c);
      if (protocol == 1) {
        if (this->rx_data_.size() < 2)
          continue;
        value = (this->rx_data_[0] << 8) + c;
      } else {
        int len = this->rx_data_.size();
        if ((len < 4) || (len < this->rx_data_[3] + 4))
          continue;
        uint8_t csum = 0;
        for (int i = 0; i < len - 2; i++)
          csum += this->rx_data_[i];
        value = NAN;
        if (csum != this->rx_data_[len - 2]) {
          ESP_LOGW(TAG, "checksum mismatch: %02x != %02x", csum, this->rx_data_[len - 2]);
        } else if (this->rx_data_[len - 1] != 0x7e) {
          ESP_LOGW(TAG, "invalid EOM");
        } else if (this->rx_data_[0] != 1) {
          ESP_LOGD(TAG, "unexpected message type %02x", this->rx_data_[0]);
        } else {
          value = (this->rx_data_[2] << 8) + this->rx_data_[3];
        }
      }
      if (!std::isnan(value) && (this->current_pos_ != value)) {
        this->current_pos_ = value;
        if (this->height_sensor_ != nullptr)
          this->height_sensor_->publish_state(value);
      }
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
