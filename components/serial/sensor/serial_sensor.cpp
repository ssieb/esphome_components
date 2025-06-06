#include "serial_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace serial {

static const char *TAG = "serial.sensor";

void SerialSensor::loop() {
  while (this->available()) {
    uint8_t c;
    this->read_byte(&c);
    this->handle_char_(c);
  }
}

void SerialSensor::handle_char_(uint8_t c) {
  if (c == '\r')
    return;
  if (c == '\n') {
    std::string s(this->rx_message_.begin(), this->rx_message_.end());
    this->publish_state(parse_number<float>(s).value_or(NAN));
    this->rx_message_.clear();
    return;
  }
  this->rx_message_.push_back(c);
}

void SerialSensor::dump_config() { LOG_SENSOR("", "Serial Sensor", this); }

}  // namespace serial
}  // namespace esphome
