#pragma once

#include "esphome/components/analog_keypad/analog_keypad.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome {
namespace analog_keypad {

class AnalogKeypadBinarySensor : public AnalogKeypadListener, public binary_sensor::BinarySensorInitiallyOff {
 public:
  void set_key(uint8_t key) {
    this->has_key_ = true;
    this->key_ = key;
  }

  void set_index(uint8_t index) {
    this->has_key_ = false;
    this->index_ = index;
  }

  void key_pressed(uint8_t key) override {
    if (!this->has_key_)
      return;
    if (key == this->key_)
      this->publish_state(true);
  }

  void key_released(uint8_t key) override {
    if (!this->has_key_)
      return;
    if (key == this->key_)
      this->publish_state(false);
  }

  void button_pressed(int index) override {
    if (this->has_key_)
      return;
    if (index == this->index_)
      this->publish_state(true);
  }

  void button_released(int index) override {
    if (this->has_key_)
      return;
    if (index == this->index_)
      this->publish_state(false);
  }

 protected:
  bool has_key_;
  uint8_t key_;
  int index_;
};

}  // namespace analog_keypad
}  // namespace esphome
