#include "mill_heater.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace mill {

/*
LED bits
0 nothing
1 Power (Red)
2 Lightning (White)
3 MinusButton (White)
4 PlusButton (White)
5 WiFiButton (White)
6 ClockButton (White)
7 WiFi (Green)
*/

static const char *const TAG = "mill_heater";

uint8_t segs[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};

float Mill::get_setup_priority() const { return setup_priority::IO; }

void Mill::setup() {
  ESP_LOGD(TAG, "Setting up Mill heater...");
  this->climate_->add_on_state_callback([this](climate::Climate &clim) { this->update_climate_state_(clim); });
}

void Mill::loop() {
  uint8_t data[7];
  if (this->read(data, 7) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "error reading from display");
    this->status_set_warning();
  }
  uint32_t now = millis();
  int key = -1;
  int touch = 0;
  if (data[5] >= 0x10) {
    key = 0;
    touch = data[5];
  }
  if ((data[6] >= 0x10) && (data[6] > touch + 5)) {
    key = 1;
    touch = data[6];
  }
  if ((data[3] >= 0x10) && (data[3] > touch + 5)) {
    key = 2;
    touch = data[3];
  }
  if ((data[4] >= 0x10) && (data[4] > touch + 5)) {
    key = 3;
    touch = data[4];
  }
  if (this->plus_key_ != nullptr)
    this->plus_key_->publish_state(key == 0);
  if (this->minus_key_ != nullptr)
    this->minus_key_->publish_state(key == 1);
  if (this->wifi_key_ != nullptr)
    this->wifi_key_->publish_state(key == 2);
  if (this->clock_key_ != nullptr)
    this->clock_key_->publish_state(key == 3);
  if (this->dark_) {
    if (key != -1)
      this->wake_start_ = now;
    else if (this->dark_override_ && (now - this->wake_start_ >= 10000))
      this->dark_override_ = false;
  }
  this->update_();
}

void Mill::update_() {
  uint8_t data[7] = {6, 5, 0, 0, 0, 0, 0};
  if (!this->dark_ || this->dark_override_) {
    if (std::isnan(this->temp_)) {
      data[2] = data[3] = data[4] = 0x40;
    } else {
      int temp = this->temp_ * 10;
      data[2] = temp >= 100 ? segs[temp / 100] : 0;
      data[3] = segs[temp / 10 % 10];
      data[4] = segs[temp % 10];
    }
    uint8_t leds = 0x78;
    if (this->power_led_)
      leds |= 2;
    if (this->lightning_led_)
      leds |= 4;
    if (this->wifi_led_)
      leds |= 0x80;
    data[5] = leds;
    data[6] = (1 << this->power_) - 1 + 0x40;
  }
  if (this->write(data, 7) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "error writing to display");
    this->status_set_warning();
  }
}

void Mill::set_power(int power) {
  if (power > 6)
    power = 6;
  this->power_ = power;
}

void Mill::update_climate_state_(climate::Climate &clim) {
  this->temp_ = clim.current_temperature;
}

}  // namespace mill
}  // namespace esphome
