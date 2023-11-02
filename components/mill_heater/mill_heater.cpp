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
}

void Mill::update_() {
  uint8_t data[7];
  data[0] = 6;
  data[1] = 5;
  int temp = this->temp_ * 10;
  data[2] = temp >= 100 ? segs[temp / 100] : 0;
  data[3] = segs[temp / 10 % 10];
  data[4] = segs[temp % 10];
  data[5] = 0;
  data[6] = (1 << this->power_) - 1 + 0x40;
  if (this->write(data, 7) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "error writing to display");
    this->status_set_warning();
  }
}

void Mill::set_temp(float temp) {
  if (temp >= 10)
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

}  // namespace mill
}  // namespace esphome
