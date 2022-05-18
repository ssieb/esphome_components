#include "sen0377.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace sen0377 {

static const char *const TAG = "sen0377";

static const uint8_t OX_REGISTER = 4;
static const uint8_t RED_REGISTER = 6;
static const uint8_t POWER_REGISTER = 8;
static const uint8_t POWER_MODE_REGISTER = 0xa;
static const uint8_t SLEEP_MODE = 0;
static const uint8_t WAKEUP_MODE = 1;
static const uint8_t OX_MODE = 0;
static const uint8_t RED_MODE = 1;

void Sen0377::setup() {
  uint8_t power_mode;
  if (this->read_register(POWER_MODE_REGISTER, &power_mode, 1) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "unable to contact sensor");
    this->mark_failed();
    return;
  }

  if (power_mode == SLEEP_MODE) {
    power_mode = WAKEUP_MODE;
    if (this->write_register(POWER_MODE_REGISTER, &power_mode, 1) != i2c::ERROR_OK) {
      ESP_LOGE(TAG, "unable to wake up sensor");
      this->mark_failed();
      return;
    }
    ESP_LOGD(TAG, "waking up sensor");
  }
  this->waking_ = true;
  this->warming_ = true;
  this->start_time_ = millis();
}

void Sen0377::loop() {
  if (this->waking_) {
    if (millis() - this->start_time_ < 100)
      return;
    this->waking_ = false;
    this->start_time_ = millis();
    return;
  }
  if (this->warming_) {
    if (millis() - this->start_time_ < 3 * 60 * 1000)  // default calibration time is 3 minutes
      return;
    uint16_t data[3];
    if (this->read_register(OX_REGISTER, (uint8_t *)&data, 6) != i2c::ERROR_OK) {
      ESP_LOGE(TAG, "unable to read initial data");
      this->mark_failed();
      return;
    }
    uint16_t power = i2c::i2ctohs(data[2]);
    this->init_ox_ = power - i2c::i2ctohs(data[0]);
    this->init_red_ = power - i2c::i2ctohs(data[1]);
    this->warming_ = false;
  }
}

void Sen0377::dump_config() {
  ESP_LOGCONFIG(TAG, "Sen0377");
  LOG_I2C_DEVICE(this);
  LOG_UPDATE_INTERVAL(this);
  LOG_SENSOR("  ", "CO Sensor", this->co_sensor_);
  LOG_SENSOR("  ", "Methane Sensor", this->methane_sensor_);
  LOG_SENSOR("  ", "Ethanol Sensor", this->ethanol_sensor_);
  LOG_SENSOR("  ", "Hydrogen Sensor", this->hydrogen_sensor_);
  LOG_SENSOR("  ", "Ammonia Sensor", this->ammonia_sensor_);
  LOG_SENSOR("  ", "NO2 Sensor", this->no2_sensor_);
  LOG_BINARY_SENSOR("  ", "CO Detect", this->co_bsensor_);
  LOG_BINARY_SENSOR("  ", "Methane Detect", this->methane_bsensor_);
  LOG_BINARY_SENSOR("  ", "Ethanol Detect", this->ethanol_bsensor_);
  LOG_BINARY_SENSOR("  ", "Propane Detect", this->propane_bsensor_);
  LOG_BINARY_SENSOR("  ", "Isobutane Detect", this->isobutane_bsensor_);
  LOG_BINARY_SENSOR("  ", "Hydrogen Detect", this->hydrogen_bsensor_);
  LOG_BINARY_SENSOR("  ", "H2S Detect", this->h2s_bsensor_);
  LOG_BINARY_SENSOR("  ", "Ammonia Detect", this->ammonia_bsensor_);
  LOG_BINARY_SENSOR("  ", "NO Detect", this->no_bsensor_);
  LOG_BINARY_SENSOR("  ", "NO2 Detect", this->no2_bsensor_);
  if (this->warming_)
    ESP_LOGD(TAG, "Sensor is calibrating");
  if (this->is_failed()) {
    ESP_LOGW(TAG, "Communication failed! Is the sensor connected?");
  }
}

void Sen0377::update() {
  if (this->warming_ || this->is_failed())
    return;

  uint16_t data[3];
  if (this->read_register(OX_REGISTER, (uint8_t *)&data, 6) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "unable to read sensor data");
    this->mark_failed();
    return;
  }
  uint16_t power = i2c::i2ctohs(data[2]);
  float ox = (power - i2c::i2ctohs(data[0])) / (float)this->init_ox_;
  float red = (power - i2c::i2ctohs(data[1])) / (float)this->init_red_;

  float value;
  if (red <= 0.425) {
    value = (0.425 - red) / 0.000405;
    if (value > 1000.0)
      value = 1000.0;
    else if (value < 1.0)
      value = 0.0;
  } else {
    value = 0.0;
  }
  if (this->co_sensor_ != nullptr)
    this->co_sensor_->publish_state(value);
  if (this->co_bsensor_ != nullptr)
    this->co_bsensor_->publish_state(value != 0.0);

  if (red <= 0.786) {
    value = (0.786 - red) / 0.000023;
    if (value < 1000.0)
      value = 0.0;
    else if (value > 25000.0)
      value = 25000.0;
  } else {
    value = 0.0;
  }
  if (this->methane_sensor_ != nullptr)
    this->methane_sensor_->publish_state(value);
  if (this->methane_bsensor_ != nullptr)
    this->methane_bsensor_->publish_state(value != 0.0);

  if (red <= 0.306) {
    value = (0.306 - red) /  0.00057;
    if (value < 10.0)
      value = 0.0;
    if (value > 500.0)
      value = 500.0;
  } else {
    value = 0.0;
  }
  if (this->ethanol_sensor_ != nullptr)
    this->ethanol_sensor_->publish_state(value);
  if (this->ethanol_bsensor_ != nullptr)
    this->ethanol_bsensor_->publish_state(value != 0.0);

  if (red <= 0.279) {
    value = (0.279 - red) / 0.00026;
    if (value < 1.0)
      value = 0.0;
    else if (value > 1000.0)
      value = 1000.0;
  } else {
    value = 0.0;
  }
  if (this->hydrogen_sensor_ != nullptr)
    this->hydrogen_sensor_->publish_state(value);
  if (this->hydrogen_bsensor_ != nullptr)
    this->hydrogen_bsensor_->publish_state(value != 0.0);

  if (red <= 0.8) {
    value = (0.8 - red) / 0.0015;
    if (value < 1.0)
      value = 0.0;
    else if (value > 500.0)
      value = 500.0;
  } else {
    value = 0.0;
  }
  if (this->ammonia_sensor_ != nullptr)
    this->ammonia_sensor_->publish_state(value);
  if (this->ammonia_bsensor_ != nullptr)
    this->ammonia_bsensor_->publish_state(value != 0.0);

  if (ox >= 1.1) {
    value = (ox - 0.045) / 6.13;
    if (value < 0.1)
      value = 0.0;
    else if (value > 10.0)
      value = 10.0;
  } else {
    value = 0.0;
  }
  if (this->no2_sensor_ != nullptr)
    this->no2_sensor_->publish_state(value);
  if (this->no2_bsensor_ != nullptr)
    this->no2_bsensor_->publish_state(value != 0.0);

  if (this->propane_bsensor_ != nullptr)
    this->propane_bsensor_->publish_state(red <= 0.18);

  if (this->isobutane_bsensor_ != nullptr)
    this->isobutane_bsensor_->publish_state(red <= 0.65);

  if (this->h2s_bsensor_ != nullptr)
    this->h2s_bsensor_->publish_state((red > 0.58 && red < 0.69) || (red < 0.201));

  if (this->no_bsensor_ != nullptr)
    this->no_bsensor_->publish_state(red <= 0.8);
}

}  // namespace sen0377
}  // namespace esphome
