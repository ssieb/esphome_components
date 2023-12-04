#include "m5_8encoder_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace m5_8encoder {

static const char *const TAG = "m5_8encoder.encoder";

void M5_8encoderSensor::setup() {
  if (this->value_ < this->min_value_)
    this->value_ = this->min_value_;
  if (this->value_ > this->max_value_)
    this->value_ = this->max_value_;
  this->publish_state(this->value_);
}

void M5_8encoderSensor::dump_config() {
  LOG_SENSOR("", "M5_8encoder rotary encoder", this);
  ESP_LOGCONFIG(TAG, "rotary encoder number %d", this->index_);
}

void M5_8encoderSensor::on_update(int32_t value) {
  int32_t new_value = this->value_ + value;
  if (new_value < this->min_value_)
    new_value = this->min_value_;
  if (new_value > this->max_value_)
    new_value = this->max_value_;
  if (new_value == this->value_)
    return;
  this->value_ = new_value;
  this->publish_state(new_value);
}

}  // namespace m5_8encoder
}  // namespace esphome
