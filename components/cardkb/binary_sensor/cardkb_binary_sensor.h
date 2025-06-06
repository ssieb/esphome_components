#pragma once

#include "esphome/components/cardkb/cardkb.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome {
namespace cardkb {

class CardKBBinarySensor : public CardKBListener, public binary_sensor::BinarySensor {
 public:
  CardKBBinarySensor(uint8_t key) : key_(key) {};

  void key_pressed(uint8_t key) override {
    if (key == this->key_)
      this->publish_state(true);
  }

  void key_released(uint8_t key) override {
    if (key == this->key_)
      this->publish_state(false);
  }

 protected:
  uint8_t key_;
};

}  // namespace cardkb
}  // namespace esphome
