#include "analog_keypad.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"

namespace esphome {
namespace analog_keypad {

static const char *const TAG = "analog_keypad";

void AnalogKeypad::loop() {
  static uint32_t active_start = 0;
  static int active_key = -1;
  uint32_t now = millis();
  int key = -1;
  bool error = false;
  int pos = 0, row, col;

  float value = this->source_->sample();
  if (std::isnan(value))
    return;

  for (int i = 0; i < this->keys_.size(); i++) {
    auto *key_info = this->keys_[i];
    if (fabs(value - key_info->voltage) <= key_info->variance) {
      key = i;
      break;
    }
  }

  if (key != active_key) {
    int pressed = this->pressed_key_;
    if ((active_key != -1) && (pressed == active_key)) {
      ESP_LOGD(TAG, "key %d released", pressed);
      for (auto &listener : this->listeners_)
        listener->button_released(pressed);
      uint8_t keycode = this->keys_[pressed]->key;
      if (keycode) {
        ESP_LOGD(TAG, "key '%c' released", keycode);
        for (auto &listener : this->listeners_)
          listener->key_released(keycode);
      }
      this->pressed_key_ = -1;
    }

    active_key = key;
    if (key == -1)
      return;
    active_start = now;
  }

  if ((this->pressed_key_ == key) || (now - active_start < this->debounce_time_))
    return;

  ESP_LOGD(TAG, "key %d pressed", key);
  for (auto &listener : this->listeners_)
    listener->button_pressed(key);
  uint8_t keycode = this->keys_[key]->key;
  if (keycode) {
    ESP_LOGD(TAG, "key '%c' pressed", keycode);
    for (auto &listener : this->listeners_)
      listener->key_pressed(keycode);
    this->send_key_(keycode);
  }
  this->pressed_key_ = key;
}

void AnalogKeypad::dump_config() {
  ESP_LOGCONFIG("", "Analog Keypad:");
}

void AnalogKeypad::add_key(float voltage, float variance, std::string key) {
  this->add_key(voltage, variance, key[0]);
}

void AnalogKeypad::add_key(float voltage, float variance, uint8_t key) {
  KeyInfo *key_info = new KeyInfo();
  key_info->voltage = voltage;
  key_info->variance = variance;
  key_info->key = key;
  this->keys_.push_back(key_info);
}

void AnalogKeypad::register_listener(AnalogKeypadListener *listener) { this->listeners_.push_back(listener); }

}  // namespace analog_keypad
}  // namespace esphome
