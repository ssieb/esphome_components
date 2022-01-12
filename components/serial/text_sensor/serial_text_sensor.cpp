#include "serial_text_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace serial {

static const char *TAG = "serial.text_sensor";

void SerialTextSensor::loop() {
  while (this->available()) {
    uint8_t c;
    this->read_byte(&c);
    this->handle_char_(c);
  }
}

void SerialTextSensor::handle_char_(uint8_t c) {
  if (c == '\r')
    return;
  if (c == '\n') {
    std::string s(this->rx_message_.begin(), this->rx_message_.end());
    this->publish_state(s);
    this->rx_message_.clear();
    return;
  }
  this->rx_message_.push_back(c);
}

void SerialTextSensor::dump_config() { LOG_TEXT_SENSOR("", "Serial Sensor", this); }

}  // namespace serial
}  // namespace esphome
