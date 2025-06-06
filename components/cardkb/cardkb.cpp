#include "cardkb.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"

namespace esphome {
namespace cardkb {

static const char *TAG = "cardkb";

void CardKB::loop() {
  uint8_t c;
  static uint32_t last_error = 0;
  c = this->pressed_key_;
  if (c) {
    for (auto &listener : this->listeners_)
      listener->key_released(c);
    this->pressed_key_ = 0;
  }
  i2c::ErrorCode res = this->read(&c, 1);
  if ((res != i2c::NO_ERROR) || (c == 0))
    return;
  if (c < 32)
    ESP_LOGD(TAG, "keycode '%d' pressed", c);
  else
    ESP_LOGD(TAG, "key '%c' pressed", c);
  for (auto &listener : this->listeners_)
    listener->key_pressed(c);
  this->pressed_key_ = c;
  this->send_key_(c);
}

void CardKB::dump_config() {
  ESP_LOGCONFIG(TAG, "CardKB:");
}

void CardKB::register_listener(CardKBListener *listener) {
  this->listeners_.push_back(listener);
}

}  // namespace cardkb
}  // namespace esphome

