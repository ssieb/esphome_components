#include "i2c_button.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace i2c_button {

enum I2C_CMD
{
  GET_STATUS = 0x01,
  RESET,
  CHANGE_I2C_ADDRESS,
  GET_KEY_VALUE,
  CHANGE_A_LONG_CLICK_TIME,
  CHANGE_B_LONG_CLICK_TIME,
  CHANGE_A_DOUBLE_CLICK_INTERVAL,
  CHANGE_B_DOUBLE_CLICK_INTERVAL
};

enum KEY_VALUE
{
    KEY_VALUE_NONE = 0x00,
    KEY_VALUE_CLICK,
    KEY_VALUE_LONG_CLICK,
    KEY_VALUE_DOUBLE_CLICK,
    KEY_VALUE_HOLD
};


static const char *const TAG = "i2c_button";

float I2C_Button::get_setup_priority() const { return setup_priority::IO; }

void I2C_Button::setup() {
  ESP_LOGD(TAG, "Setting up i2c button...");
  uint8_t versions[2];
  this->read_bytes(GET_STATUS, versions, 2);
  ESP_LOGD(TAG, "Product ID: %d  Version: %d", versions[0], versions[1]);
}

void I2C_Button::loop() {
  uint8_t buttons = this->reg(GET_KEY_VALUE).get();
  if (buttons == 0)
    return;
  uint8_t a = buttons >> 4;
  uint8_t b = buttons & 0xf;
  switch (a) {
   case KEY_VALUE_CLICK:
    ESP_LOGD(TAG, "Button A clicked");
    this->a_click_trigger_->trigger();
    break;
   case KEY_VALUE_LONG_CLICK:
    ESP_LOGD(TAG, "Button A long clicked");
    this->a_long_click_trigger_->trigger();
    break;
   case KEY_VALUE_DOUBLE_CLICK:
    ESP_LOGD(TAG, "Button A double clicked");
    this->a_double_click_trigger_->trigger();
    break;
   case KEY_VALUE_HOLD:
    ESP_LOGD(TAG, "Button A held");
    this->a_hold_trigger_->trigger();
    break;
  }
  switch (b) {
   case KEY_VALUE_CLICK:
    ESP_LOGD(TAG, "Button B clicked");
    this->b_click_trigger_->trigger();
    break;
   case KEY_VALUE_LONG_CLICK:
    ESP_LOGD(TAG, "Button B long clicked");
    this->b_long_click_trigger_->trigger();
    break;
   case KEY_VALUE_DOUBLE_CLICK:
    ESP_LOGD(TAG, "Button B double clicked");
    this->b_double_click_trigger_->trigger();
    break;
   case KEY_VALUE_HOLD:
    ESP_LOGD(TAG, "Button B held");
    this->b_hold_trigger_->trigger();
    break;
  }
}


}  // namespace i2c_button
}  // namespace esphome
