#include "wts01.h"
#include "esphome/core/log.h"

namespace esphome {
namespace wts01 {

static const char *TAG = "wts01";

void WTS01::dump_config() {
  LOG_SENSOR("", "WTS01", this);
  this->check_uart_settings(9600);
}

void WTS01::loop() {
  const uint32_t now = millis();
  if (this->reading_ && (now - this->last_transmission_ >= 500)) {
    ESP_LOGW(TAG, "Last transmission too long ago. Reset buffer.");
    this->data_.clear();
    this->reading_ = false;
  }

  if (!this->available())
    return;

  this->last_transmission_ = now;
  while (this->available()) {
    uint8_t c;
    this->read_byte(&c);
    if (!reading_) {
      if (c == 0x55)
        this->reading_ = true;
      continue;
    }
    this->data_.push_back(c);
    if (this->data_.size() < 8)
      continue;
    uint8_t csum = 0x55;
    uint8_t dsum = this->data_[7];
    for (int i = 0; i < 7; i++)
      csum += this->data_[i];
    float temp = this->data_[5] + this->data_[6] / 100.0f;
    this->data_.clear();
    this->reading_ = false;
    if (csum != dsum) {
      ESP_LOGE(TAG, "Checksum mismatch: %02x != %02x", csum, dsum);
      continue;
    }
    this->publish_state(temp);
  }
}

}  // namespace wts01
}  // namespace esphome
