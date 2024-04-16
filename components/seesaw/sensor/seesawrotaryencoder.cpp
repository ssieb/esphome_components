#include "seesawrotaryencoder.h"
#include "esphome/core/log.h"

namespace esphome {
namespace seesaw {

static const char *const TAG = "seesaw.encoder";

void SeesawRotaryEncoder::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Seesaw rotary encoder...");
  this->parent_->enable_encoder(this->number_);
  this->publish_state(0);
}

void SeesawRotaryEncoder::dump_config() {
  ESP_LOGCONFIG("", "Seesaw Encoder Sensor", this);
  ESP_LOGCONFIG(TAG, "  Pin: %d", this->pin_);
}

void SeesawRotaryEncoder::loop() {
  int32_t new_value = this->parent_->get_encoder_position(this->number_);
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
