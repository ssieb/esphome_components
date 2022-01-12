#include "keypad_text_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace keypad {

static const char *TAG = "keypad_text_sensor";

KeypadTextSensor::KeypadTextSensor() : progress_trigger_(new Trigger<std::string>()) {}

void KeypadTextSensor::dump_config() {
  ESP_LOGCONFIG(TAG, "Keypad text sensor");
  if (this->max_length_ > 0)
    ESP_LOGCONFIG(TAG, "  max length: %d", this->max_length_);
  if (!this->back_keys_.empty())
    ESP_LOGCONFIG(TAG, "  erase keys '%s'", this->back_keys_.c_str());
  if (!this->end_keys_.empty())
    ESP_LOGCONFIG(TAG, "  end keys '%s'", this->end_keys_.c_str());
  if (!this->allowed_keys_.empty())
    ESP_LOGCONFIG(TAG, "  allowed keys '%s'", this->allowed_keys_.c_str());
}

void KeypadTextSensor::key_pressed(unsigned char key) {
  if (this->back_keys_.find(key) != std::string::npos) {
    if (!this->result_.empty()) {
      this->result_.pop_back();
      this->progress_trigger_->trigger(this->result_);
    }
    return;
  }
  if (this->end_keys_.find(key) != std::string::npos) {
    this->publish_state(this->result_);
    this->result_.clear();
    this->progress_trigger_->trigger(this->result_);
    return;
  }
  if (!this->allowed_keys_.empty() && (this->allowed_keys_.find(key) == std::string::npos))
    return;
  this->result_.push_back(key);
  if ((this->max_length_ > 0) && (this->result_.size() == this->max_length_)) {
    this->publish_state(this->result_);
    this->result_.clear();
  }
  this->progress_trigger_->trigger(this->result_);
}

Trigger<std::string> *KeypadTextSensor::get_progress_trigger() const { return this->progress_trigger_; };

}  // namespace keypad
}  // namespace esphome

