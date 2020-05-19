#include "ezo.h"
#include "esphome/core/log.h"

namespace esphome {
namespace ezo {

static const char *TAG = "ezo.sensor";

void EZOSensor::dump_config() {
  LOG_SENSOR("", "EZO", this);
  LOG_I2C_DEVICE(this);
  if (this->is_failed())
    ESP_LOGE(TAG, "Communication with EZO circuit failed!");
  LOG_UPDATE_INTERVAL(this);
}

void EZOSensor::update() {
  if (this->waiting_) {
    ESP_LOGE(TAG, "update overrun, still waiting for previous response");
    return;
  }
  uint8_t c = 'R';
  this->write_bytes_raw(&c, 1);
  this->waiting_ = true;
  this->start_time_ = millis();
}

void EZOSensor::loop() {
  if (!this->waiting_ || (millis() - this->start_time_ < 900))
    return;
  static uint8_t buf[20];
  buf[0] = 0;
  if (!this->read_bytes_raw(buf, 20)) {
    ESP_LOGE(TAG, "read error");
    this->waiting_ = false;
    return;
  }
  switch(buf[0]) {
   case 1:
    break;
   case 2:
    ESP_LOGE(TAG, "device returned a syntax error");
    break;
   case 254:
    return; // keep waiting
   case 255:
    ESP_LOGE(TAG, "device returned no data");
    break;
   default:
    ESP_LOGE(TAG, "device returned an unknown response: %d", buf[0]);
    break;
  }
  this->waiting_ = false;
  if (buf[0] != 1)
    return;

  float val = atof((char *)&buf[1]);
  this->publish_state(val);
}

}  // namespace ezo
}  // namespace esphome
