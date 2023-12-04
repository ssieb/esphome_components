#include "m5_8encoder.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace m5_8encoder {

static const char *const TAG = "m5_8encoder";

static const uint8_t COUNTER_BASE = 0x00;
static const uint8_t INCREMENT_BASE = 0x20;
static const uint8_t RESET_BASE = 0x40;
static const uint8_t BUTTON_BASE = 0x50;
static const uint8_t TOGGLE = 0x60;
static const uint8_t RGB_BASE = 0x70;
static const uint8_t VERSION = 0xfe;
static const uint8_t ADDRESS = 0xff;

void M5_8encoder::setup() {
  ESP_LOGCONFIG(TAG, "Setting up M5_8encoder...");
  if (this->read_register(VERSION, &this->version_, 1) != i2c::ERROR_OK) {
    this->mark_failed();
    return;
  }
  ESP_LOGCONFIG(TAG, "Firmware version %d", this->version_);
  uint8_t data[8] = {0};
  this->write_register(RESET_BASE, data, 8);
}

void M5_8encoder::loop() {
  uint8_t data[8 * 4];
  if (this->read_register(INCREMENT_BASE, data, 8 * 4) != i2c::ERROR_OK) {
    this->mark_failed();
    return;
  }
  for (auto *listener : this->encoder_listeners_) {
    int32_t value = *((int32_t *)(data + listener->get_index() * 4));
    listener->on_update(value);
  }
  if (this->read_register(TOGGLE, data, 1) != i2c::ERROR_OK) {
    this->mark_failed();
    return;
  }
  for (auto *listener : this->toggle_listeners_)
    listener->on_update(data[0]);
}

void M5_8encoder::dump_config() {
  ESP_LOGCONFIG(TAG, "M5_8encoder:");
  if (this->version_ > 0)
    ESP_LOGCONFIG(TAG, "Firmware version %d", this->version_);
  else
    ESP_LOGCONFIG(TAG, "setup failed");
}

void M5_8encoder::set_led(uint8_t led, uint8_t red, uint8_t green, uint8_t blue) {
  if (led > 8)
    return;
  uint8_t data[3] = {red, green, blue};
  this->write_register(RGB_BASE + led * 3, data, 3);
}

}  // namespace m5_8encoder
}  // namespace esphome
