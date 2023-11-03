#include "mill_heater.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace mill {

static const char *const TAG = "mill_heater";

uint8_t segs[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};

float Mill::get_setup_priority() const { return setup_priority::IO; }

void Mill::setup() {
  ESP_LOGD(TAG, "Setting up Mill heater...");
  this->update_();
}

void Mill::loop() {
  uint8_t data[7];
  if (this->read(data, 7) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "error reading from display");
    this->status_set_warning();
  }
  int key = -1;
  int touch = 0;
  if (data[5] >= 0x10) {
    key = 0;
    touch = data[5];
  }
  if ((data[6] >= 0x10) && (data[6] > touch)) {
    key = 1;
    touch = data[6];
  }
  if ((data[3] >= 0x10) && (data[3] > touch)) {
    key = 2;
    touch = data[3];
  }
  if ((data[4] >= 0x10) && (data[4] > touch)) {
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
}

void Mill::update_() {
  uint8_t data[7];
  data[0] = 6;
  data[1] = 5;
  int temp = this->temp_ * 10;
  data[2] = temp >= 100 ? segs[temp / 100] : 0;
  data[3] = segs[temp / 10 % 10];
  data[4] = segs[temp % 10];
  data[5] = 1 << this->bnum_;
  data[6] = (1 << this->power_) - 1 + 0x40;
  if (this->write(data, 7) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "error writing to display");
    this->status_set_warning();
  }
}

void Mill::set_temp(float temp) {
  if (temp >= 100)
    temp = 99.9;
  this->temp_ = temp;
  this->update_();
}

void Mill::set_power(int power) {
  if (power > 6)
    power = 6;
  this->power_ = power;
  this->update_();
}

void Mill::set_button(int bnum) {
  if (bnum > 7)
    bnum = 7;
  this->bnum_ = bnum;
  this->update_();
}

}  // namespace mill
}  // namespace esphome
