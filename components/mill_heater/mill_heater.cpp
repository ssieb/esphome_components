#include "mill_heater.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

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
  int key = 0;
  int touch = 0;
  if (data[5] >= 0x10) {
    key = 1;
    touch = data[5];
  }
  if ((data[6] >= 0x10) && (data[6] > touch + 5)) {
    key = 2;
    touch = data[6];
  }
  if ((data[3] >= 0x10) && (data[3] > touch + 5)) {
    key = 3;
    touch = data[3];
  }
  if ((data[4] >= 0x10) && (data[4] > touch + 5)) {
    key = 4;
    touch = data[4];
  }
  if (this->dark_) {
    if (key > 0) {
      this->wake_start_ = now;
      if (!this->dark_override_) {
        this->dark_override_ = true;
        this->update_();
        return;
      }
    } else if (this->dark_override_ && (now - this->wake_start_ >= this->dark_timeout_)) {
      this->dark_override_ = false;
      this->update_();
      return;
    }
  }
  if (key != this->current_key_) {
    switch (this->current_key_) {
      case 1: // plus key
      case 2: // minus key
        this->key_release_ = now;
        break;
      case 3: // wifi key
        if (this->wifi_key_ != nullptr)
          this->wifi_key_->publish_state(false);
        break;
      case 4: // clock key
        if (this->clock_key_ != nullptr)
          this->clock_key_->publish_state(false);
        break;
    }
    switch (key) {
      case 1: // plus key
      case 2: // minus key
        this->key_start_ = now;
        this->key_release_ = 0;
        this->key_last_repeat_ = 0;
        if (!this->blinking_) {
          this->blinking_ = true;
          this->blink_on_ = true;
          this->last_blink_ = now;
          break;
        }
        this->adjust_target_temp_(key);
        break;
      case 3: // wifi key
        if (this->wifi_key_ != nullptr)
          this->wifi_key_->publish_state(true);
        break;
      case 4: // clock key
        if (this->clock_key_ != nullptr)
          this->clock_key_->publish_state(true);
    }
    this->current_key_ = key;
  } else {
    if ((key == 1) || (key == 2)) {
      if (this->key_last_repeat_) {
        if (now - this->key_last_repeat_ >= this->key_repeat_interval_)
          this->adjust_target_temp_(key);
      } else if (now - this->key_start_ >= this->key_repeat_delay_) {
        this->key_last_repeat_ = now;
        this->adjust_target_temp_(key);
      }
    }
  }
  if (this->blinking_) {
    if (now - this->last_blink_ >= this->blink_interval_)
      this->blink_on_ = !this->blink_on_;
    if (this->key_release_ && (now - this->key_release_ >= this->set_temp_delay_))
      this->blinking_ = false;
  }
  this->update_();
}

void Mill::update_() {
  uint8_t data[7] = {6, 5, 0, 0, 0, 0, 0};
  if (!this->dark_ || this->dark_override_) {
    float temp = this->blinking_ ? this->target_temp_ : this->temp_;
    if (std::isnan(temp_)) {
      data[2] = data[3] = data[4] = 0x40;
    } else if (!this->blinking_ || this->blink_on_) {
      int itemp = this->temp_ * 10;
      data[2] = itemp >= 100 ? segs[itemp / 100] : 0;
      data[3] = segs[itemp / 10 % 10];
      data[4] = segs[itemp % 10];
    }
    uint8_t leds = this->blinking_ ? 0x18 : 0x78;
    if (this->power_led_)
      leds |= 2;
    if (this->lightning_led_)
      leds |= 4;
    if (this->wifi_led_)
      leds |= 0x80;
    data[5] = leds;
    data[6] = (1 << this->power_) - 1;
    data[6] |= 0x40; // decimal point
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
  this->target_temp_ = clim.target_temperature;
  this->power_led_ = clim.mode != climate::CLIMATE_MODE_OFF;
  this->lightning_led_ = clim.action != climate::CLIMATE_ACTION_OFF;
}

void Mill::adjust_target_temp_(uint8_t key) {
  if (this->climate_ == nullptr)
    return;
  float target = this->target_temp_ + (key == 1 ? 0.1f : -0.1f);
  this->climate_->make_call().set_target_temperature_low(target).perform();
}

}  // namespace mill
}  // namespace esphome
