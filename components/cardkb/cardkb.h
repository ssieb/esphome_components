#pragma once

#include "esphome/components/i2c/i2c.h"
#include "esphome/components/key_provider/key_provider.h"
#include "esphome/core/component.h"

namespace esphome {
namespace cardkb {

class CardKBListener {
 public:
  virtual void key_pressed(uint8_t key) {};
  virtual void key_released(uint8_t key) {};
};

class CardKB : public key_provider::KeyProvider, public Component, public i2c::I2CDevice {
 public:
  void loop() override;
  void dump_config() override;

  void register_listener(CardKBListener *listener);

 protected:
  int pressed_key_ = -1;

  std::vector<CardKBListener *> listeners_{};
};

}  // namespace cardkb
}  // namespace esphome


