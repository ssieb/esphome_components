#include "input_builder.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"

namespace esphome {
namespace input_builder {

static const char *TAG = "input_builder";

InputBuilder::InputBuilder() : progress_trigger_(new Trigger<std::string>()), result_trigger_(new Trigger<std::string>()) {}

void InputBuilder::loop() {
  if ((this->timeout_ == 0) || (this->result_.size() == 0) || (millis() - this->last_key_time_ < this->timeout_))
    return;
  this->result_.clear();
  this->progress_trigger_->trigger(this->result_);
}

void InputBuilder::dump_config() {
  ESP_LOGCONFIG(TAG, "Input builder");
  if (this->min_length_ > 0)
    ESP_LOGCONFIG(TAG, "  min length: %d", this->min_length_);
  if (this->max_length_ > 0)
    ESP_LOGCONFIG(TAG, "  max length: %d", this->max_length_);
  if (!this->back_keys_.empty())
    ESP_LOGCONFIG(TAG, "  erase keys '%s'", this->back_keys_.c_str());
  if (!this->clear_keys_.empty())
    ESP_LOGCONFIG(TAG, "  clear keys '%s'", this->clear_keys_.c_str());
  if (!this->start_keys_.empty()) {
    ESP_LOGCONFIG(TAG, "  start keys '%s'", this->start_keys_.c_str());
  if (!this->end_keys_.empty()) {
    ESP_LOGCONFIG(TAG, "  end keys '%s'", this->end_keys_.c_str());
    ESP_LOGCONFIG(TAG, "  end key is required: %s", ONOFF(this->end_key_required_));
  }
  if (!this->allowed_keys_.empty())
    ESP_LOGCONFIG(TAG, "  allowed keys '%s'", this->allowed_keys_.c_str());
  if (this->timeout_ > 0)
    ESP_LOGCONFIG(TAG, "  entry timeout: %0.1f", this->timeout_ / 1000.0);
}

void InputBuilder::set_provider(key_provider::KeyProvider *provider) {
  provider->add_on_key_callback([this](uint8_t key) {
    this->key_pressed_(key);
  });
}
  
bool InputBuilder::can_handle_(uint8_t key) {
  if (!this->is_started_ ) 
    this->is_started_  = this->back_keys_.find(key) != std::string::npos);
  return this->is_started_;
}
  
void InputBuilder::key_pressed_(uint8_t key) {
  this->last_key_time_ = millis();
  if (!this->end_keys_.empty() && !this->can_handle_(key)) 
    return;
  if (this->back_keys_.find(key) != std::string::npos) {
    if (!this->result_.empty()) {
      this->result_.pop_back();
      this->progress_trigger_->trigger(this->result_);
    }
    return;
  }
  if (this->clear_keys_.find(key) != std::string::npos) {
    if (!this->result_.empty()) {
      this->result_.clear();
      this->progress_trigger_->trigger(this->result_);
    }
    this->is_started = false;
    return;
  }
  if (this->end_keys_.find(key) != std::string::npos) {
    if ((this->min_length_ == 0) || (this->result_.size() >= this->min_length_)) {
      this->result_trigger_->trigger(this->result_);
      this->result_.clear();
      this->progress_trigger_->trigger(this->result_);
    }
    this->is_started = false;
    return;
  }
  if (!this->allowed_keys_.empty() && (this->allowed_keys_.find(key) == std::string::npos))
    return;
  if ((this->max_length_ == 0) || (this->result_.size() < this->max_length_))
    this->result_.push_back(key);
  if ((this->max_length_ > 0) && (this->result_.size() == this->max_length_) && (!this->end_key_required_)) {
    this->result_trigger_->trigger(this->result_);
    this->result_.clear();
    this->is_started = false;
  }
  this->progress_trigger_->trigger(this->result_);
}

}  // namespace input_builder
}  // namespace esphome

