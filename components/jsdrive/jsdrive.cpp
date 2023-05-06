#include "jsdrive.h"
#include "esphome/core/log.h"

namespace esphome {
namespace jsdrive {

static const char *const TAG = "jsdrive";

const char *jsdrive_operation_to_str(JSDriveOperation op) {
  switch (op) {
    case JSDRIVE_OPERATION_IDLE:
      return "IDLE";
    case JSDRIVE_OPERATION_RAISING:
      return "RAISING";
    case JSDRIVE_OPERATION_LOWERING:
      return "LOWERING";
    default:
      return "UNKNOWN";
  }
}

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
  if (this->desk_uart_ != nullptr) {
    while (this->desk_uart_->available()) {
      this->desk_uart_->read_byte(&c);
      if (this->remote_uart_ != nullptr)
        this->remote_uart_->write_byte(c);
      if (!this->desk_rx_) {
        if (c != 0x5a)
          continue;
        this->desk_rx_ = true;
        continue;
      }
      this->desk_buffer_.push_back(c);
      if (this->desk_buffer_.size() < this->message_length_ - 1)
        continue;
      this->desk_rx_ = false;
      uint8_t *d = this->desk_buffer_.data();
      uint8_t csum = d[0] + d[1] + d[2];
      if (this->message_length_ > 5)
        csum += d[3];
      uint8_t tcsum = this->message_length_ == 5 ? d[3] : d[4];
      if (csum != tcsum) {
        ESP_LOGE(TAG, "desk checksum mismatch: %02x != %02x", csum, tcsum);
        this->desk_buffer_.clear();
        continue;
      }
      if (this->height_sensor_ != nullptr) {
        do {
          if ((this->message_length_ == 6) && (d[3] != 1)) {
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
          this->current_pos_ = num;
          this->height_sensor_->publish_state(num);
        } while (false);
      }
      this->desk_buffer_.clear();
    }
  }
  if (this->moving_) {
    if ((this->move_dir_ && (this->current_pos_ >= this->target_pos_)) ||
        (!this->move_dir_ && (this->current_pos_ <= this->target_pos_))) {
      this->moving_ = false;
    } else if (millis() - this->last_send_ > 200) {
      static uint8_t buf[] = {0xa5, 0, 0, 0, 0xff};
      buf[2] = (this->move_dir_ ? 0x20 : 0x40);
      buf[3] = 0xff - buf[2];
      this->desk_uart_->write_array(buf, 5);
    }
  }
  if (this->remote_uart_ != nullptr) {
    while (this->remote_uart_->available()) {
      this->remote_uart_->read_byte(&c);
      if (!this->moving_ && this->desk_uart_ != nullptr)
        this->desk_uart_->write_byte(c);
      if (!this->rem_rx_) {
        if (c != 0xa5)
          continue;
        this->rem_rx_ = true;
        continue;
      }
      this->rem_buffer_.push_back(c);
      if (this->rem_buffer_.size() < 4)
        continue;
      this->rem_rx_ = false;
      uint8_t *d = this->rem_buffer_.data();
      uint8_t csum = d[0] + d[1] + d[2];
      if (csum != d[3]) {
        ESP_LOGE(TAG, "remote checksum mismatch: %02x != %02x", csum, d[3]);
        this->desk_buffer_.clear();
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
}

void JSDrive::dump_config() {
  ESP_LOGCONFIG(TAG, "JSDrive Desk");
  if (this->desk_uart_ != nullptr)
    ESP_LOGCONFIG(TAG, "  Message Length: %d", this->message_length_);
  LOG_SENSOR("", "Height", this->height_sensor_);
  LOG_BINARY_SENSOR("  ", "Up", this->up_bsensor_);
  LOG_BINARY_SENSOR("  ", "Down", this->down_bsensor_);
  LOG_BINARY_SENSOR("  ", "Memory1", this->memory1_bsensor_);
  LOG_BINARY_SENSOR("  ", "Memory2", this->memory2_bsensor_);
  LOG_BINARY_SENSOR("  ", "Memory3", this->memory3_bsensor_);
}

void JSDrive::move_to(float height) {
  if (this->desk_uart_ == nullptr)
    return;
  this->moving_ = true;
  this->target_pos_ = height;
  this->move_dir_ = height > this->current_pos_;
  this->current_operation = this->move_dir_ ? JSDRIVE_OPERATION_RAISING : JSDRIVE_OPERATION_LOWERING;
}

void JSDrive::stop() {
  this->moving_ = false;
  this->current_operation = JSDRIVE_OPERATION_IDLE;
}

}  // namespace jsdrive
}  // namespace esphome
