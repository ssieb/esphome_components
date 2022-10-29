#include "t67xx.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace t67xx {

static const char *const TAG = "t67xx";

void T67xx::dump_config() {
  LOG_SENSOR("", "T67XX", this);
  LOG_I2C_DEVICE(this);
  LOG_UPDATE_INTERVAL(this);
}

void T67xx::update() {
  if (this->is_failed())
    return;

  static uint8_t status[5] = {0x04, 0x13, 0x8a, 0x00, 0x01};
  static uint8_t gas_ppm[5] = {0x04, 0x13, 0x8b, 0x00, 0x01};
  uint8_t data[4];
  if (this->write(status, 5) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "error writing to sensor");
    this->mark_failed();
    return;
  }
  if (this->read(data, 4) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "error reading status");
    this->mark_failed();
    return;
  }
  if (data[3] & 2) {
    ESP_LOGE(TAG, "flash error");
    this->mark_failed();
    return;
  }
  if (data[3] & 4) {
    ESP_LOGE(TAG, "calibration error");
    this->start_calibration();
    return;
  }
  if (data[3] & 1) {
    ESP_LOGE(TAG, "unkown error");
    this->mark_failed();
    return;
  }
  if (data[2] & 8) {
    ESP_LOGD(TAG, "still warming up");
    return;
  }
  if (data[2] & 0x80) {
    ESP_LOGD(TAG, "calibrating");
    return;
  }

  if (this->write(gas_ppm, 5) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "error writing to sensor");
    this->mark_failed();
    return;
  }
  if (this->read(data, 4) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "error reading data");
    this->mark_failed();
    return;
  }

  this->publish_state((data[2] << 8) + data[3]);
}

void T67xx::start_calibration() {
  static uint8_t calibrate[5] = {0x05, 0x03, 0xec, 0xff, 0x00};
  uint8_t data[5];
  ESP_LOGD("starting calibration");
  if (this->write(calibrate, 5) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "error writing to sensor");
    this->mark_failed();
    return;
  }
  if (this->read(data, 5) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "error reading data");
    this->mark_failed();
    return;
  }
}

}  // namespace t67xx
}  // namespace esphome
