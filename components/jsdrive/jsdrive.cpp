#include "jsdrive.h"
#include "esphome/core/log.h"

namespace esphome {
namespace jsdrive {

static const char *const TAG = "jsdrive";

static int segs_to_num(uint8_t segments) {
  switch (segments & 0x7f) {
   case 0x3f:
    return 0;
   case 0x06:
    return 1;
   case 0x5b:
    return 2;
   case 0x4f:
    return 3;
   case 0x67:
    return 4;
   case 0x6d:
    return 5;
   case 0x7d:
    return 6;
   case 0x07:
    return 7;
   case 0x7f:
    return 8;
   case 0x6f:
    return 9;
   default:
    ESP_LOGV(TAG, "unknown digit: %02f", segments & 0x7f);
  }
  return -1;
}

void JSDrive::loop() {
  uint8_t c;
  if (this->remote_uart_ != nullptr) {
    while (this->remote_uart_->available()) {
      this->remote_uart_->read_byte(&c);
      if (!this->rem_rx_) {
        if (c != 0x5a)
          continue;
        this->rem_rx_ = true;
        continue;
      }
      this->rem_buffer_.push_back(c);
      if (this->rem_buffer_.size() < 5)
        continue;
      this->rem_rx_ = false;
      uint8_t *d = this->rem_buffer_.data();
      if ((d[3] != 0xff) || (d[4] != 0xff)) {
        ESP_LOGE(TAG, "unexpected end bytes: %02x %02x", d[3], d[4]);
        this->rem_buffer_.clear();
        continue;
      }
      if (this->up_bsensor_ != nullptr)
        this->up_bsensor_->publish_state(d[1] & 0x20);
      if (this->down_bsensor_ != nullptr)
        this->down_bsensor_->publish_state(d[1] & 0x40);
      if (this->memory1_bsensor_ != nullptr)
        this->memory1_bsensor_->publish_state(d[1] & 2);
      if (this->memory2_bsensor_ != nullptr)
        this->memory2_bsensor_->publish_state(d[1] & 4);
      if (this->memory3_bsensor_ != nullptr)
        this->memory3_bsensor_->publish_state(d[1] & 8);
      this->rem_buffer_.clear();
    }
  }
  if (this->desk_uart_ != nullptr) {
    while (this->desk_uart_->available()) {
      this->desk_uart_->read_byte(&c);
      if (!this->desk_rx_) {
        if (c != 0x5a)
          continue;
        this->desk_rx_ = true;
        continue;
      }
      this->desk_buffer_.push_back(c);
      if (this->desk_buffer_.size() < 5)
        continue;
      this->desk_rx_ = false;
      uint8_t *d = this->desk_buffer_.data();
      uint8_t csum = d[0] + d[1] + d[2] + d[3];
      if (csum != d[4]) {
        ESP_LOGE(TAG, "desk checksum mismatch: %02x != %02x", csum, d[4]);
        this->desk_buffer_.clear();
        continue;
      }
      if (this->height_sensor_ != nullptr) {
        do {
          if (d[3] != 1) {
            ESP_LOGV(TAG, "unknown message type %02x", d[3]);
            break;
          }
          if ((d[0] | d[1] | d[2]) == 0)
            break;
          int d0 = segs_to_num(d[0]);
          int d1 = segs_to_num(d[1]);
          int d2 = segs_to_num(d[2]);
          if (d0 < 0 || d1 < 0 || d2 < 0)
            break;
          float num = segs_to_num(d[0]) * 100 + segs_to_num(d[1]) * 10 + segs_to_num(d[2]);
          if (d[1] & 0x80)
            num /= 10.0;
          this->height_sensor_->publish_state(num);
        } while (false);
      }
      this->desk_buffer_.clear();
    }
  }
}

void JSDrive::dump_config() {
  ESP_LOGCONFIG(TAG, "JSDrive Desk");
  LOG_SENSOR("", "Height", this->height_sensor_);
  LOG_BINARY_SENSOR("  ", "Up", this->up_bsensor_);
  LOG_BINARY_SENSOR("  ", "Down", this->down_bsensor_);
  LOG_BINARY_SENSOR("  ", "Memory1", this->memory1_bsensor_);
  LOG_BINARY_SENSOR("  ", "Memory2", this->memory2_bsensor_);
  LOG_BINARY_SENSOR("  ", "Memory3", this->memory3_bsensor_);
}

}  // namespace jsdrive
}  // namespace esphome
