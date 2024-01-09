#pragma once

#include "esphome/components/analog_keypad/analog_keypad.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome {
namespace analog_keypad {

class AnalogKeypadBinarySensor : public AnalogKeypadListener, public binary_sensor::BinarySensor {
 public:
  AnalogKeypadBinarySensor(const char *key) : has_key_(true), key_((uint8_t) key[0]){};
  AnalogKeypadBinarySensor(uint8_t index) : has_key_(false), index_(index){};

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
