#include "rfid_text_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace rfid {

static const char *TAG = "rfid.text_sensor";

void RFIDTextSensor::loop() {
  while (this->available()) {
    uint8_t c;
    this->read_byte(&c);
    this->handle_char_(c);
  }
}

void RFIDTextSensor::handle_char_(uint8_t c) {
  if (c == 1) {
    this->rx_message_.clear();
    return;
  }
  if (c == 2) {
    std::string s(this->rx_message_.begin(), this->rx_message_.end());
    this->publish_state(s);
    this->rx_message_.clear();
    return;
  }
  if (rx_message_.size() < 20)
    this->rx_message_.push_back(c);
}

void RFIDTextSensor::dump_config() { LOG_TEXT_SENSOR("", "RFID Sensor", this); }

}  // namespace rfid
}  // namespace esphome
