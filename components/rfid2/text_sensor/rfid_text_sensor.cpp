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
  static int state = 0;

  if (c == '$') {
    this->rx_message_.clear();
    state = 1;
    return;
  }
  if (state == 1 && c == 'F') {
    state = 2;
    return;
  }
  if (c == '#') {
    if (state == 2 && this->rx_message_.size() > 2) {
      uint8_t n2 = this->rx_message_.back() - '0';
      this->rx_message_.pop_back();
      uint8_t n1 = this->rx_message_.back() - '0';
      this->rx_message_.pop_back();
      if (n1 > 9)
        n1 -= 7;
      if (n2 > 9)
        n2 -= 7;
      uint8_t csum = 'F';
      for (uint8_t x: this->rx_message_)
        csum ^= x;
      if (csum == ((n1 << 4) + n2)) {
        std::string s(this->rx_message_.begin(), this->rx_message_.end());
        this->publish_state(s);
      } else {
        ESP_LOGW(TAG, "checksum failed: %02x != %02x", csum, (n1 << 4) + n2);
      }
      this->rx_message_.clear();
    }
    state = 0;
    return;
  }
  if (rx_message_.size() < 30)
    this->rx_message_.push_back(c);
}

void RFIDTextSensor::dump_config() { LOG_TEXT_SENSOR("", "RFID Sensor", this); }

}  // namespace rfid
}  // namespace esphome
