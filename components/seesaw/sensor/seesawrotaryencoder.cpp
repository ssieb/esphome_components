#include "seesawrotaryencoder.h"
#include "esphome/core/log.h"

namespace esphome {
namespace seesaw {

static const char *const TAG = "seesaw_encoder";

void SeesawRotaryEncoder::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Seesaw rotary encoder...");
  this->parent_->enable_encoder();
  this->publish_state(0);
}

void SeesawRotaryEncoder::loop() {
  int32_t new_value = -this->parent_->get_encoder_position();
  if (new_value < this->min_value_)
    new_value = this->min_value_;
  if (new_value > this->max_value_)
    new_value = this->max_value_;
  if (new_value == this->value_)
    return;
  this->value_ = new_value;
  this->publish_state(new_value);
}

}  // namespace seesaw
}  // namespace esphome
