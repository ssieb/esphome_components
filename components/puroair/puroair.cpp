#include "puroair.h"
#include "esphome/core/log.h"

namespace esphome {
namespace puroair {

static const char *const TAG = "puroair";

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

static void log_state(uint32_t state) {
  switch(state) {
   case 0x080008:
    ESP_LOGD(TAG, "State: Power off");
    break;
   case 0x480001:
   case 0x480002:
    ESP_LOGD(TAG, "State: Fan off");
    break;
   case 0x280001:
    ESP_LOGD(TAG, "State: Fan low");
    break;
   case 0x0c0002:
    ESP_LOGD(TAG, "State: Fan med");
    break;
   case 0x100000:
    ESP_LOGD(TAG, "State: Fan sleep");
    break;
   default:
    ESP_LOGD(TAG, "State: Unknown");
  }
}

void PuroAir::loop() {
  uint8_t c;
  bool have_data = false;
  if (this->control_uart_ != nullptr) {
    while (this->control_uart_->available()) {
      this->control_uart_->read_byte(&c);
      if (!this->control_rx_) {
        if (c != 0xd2)
          continue;
        this->control_rx_ = true;
        continue;
      }
      if (this->control_buffer_.size() < 3) {
        this->control_buffer_.push_back(c);
        continue;
      }
      uint8_t csum = 0xd2 + this->control_buffer_[0] + this->control_buffer_[1] + this->control_buffer_[2];
      uint32_t new_state = (this->control_buffer_[0] << 16) + (this->control_buffer_[1] << 8) + this->control_buffer_[0];
      this->control_buffer_.clear();
      this->control_rx_ = false;
      if (csum != c) {
        ESP_LOGW(TAG, "controller checksum mismatch: %02x != %02x", csum, c);
        continue;
      }
      if (new_state != this->cstate_) {
        this->cstate_ = new_state;
        ESP_LOGD(TAG, "new controller state is %06x", new_state);
      }
    }
  }

  if (this->button_uart_ != nullptr) {
    while (this->button_uart_->available()) {
      this->button_uart_->read_byte(&c);
      if (!this->button_rx_) {
        if (c != 0xd2)
          continue;
        this->button_rx_ = true;
        continue;
      }
      if (this->new_bstate_ == 0xff) {
        this->new_bstate_ = c;
        continue;
      }
      uint8_t csum = 0xd2 + this->new_bstate_;
      uint8_t new_state = this->new_bstate_;
      this->new_bstate_ = 0xff;
      this->button_rx_ = false;
      if (csum != c) {
        ESP_LOGW(TAG, "button checksum mismatch: %02x != %02x", csum, c);
        continue;
      }
      if (new_state != this->bstate_) {
        this->bstate_ = new_state;
        ESP_LOGD(TAG, "new button state is %02x", new_state);
        log_state(new_state);
      }
    }
  }

  uint32_t now = millis();
  if (now - this->last_send_ < 100)
    return;
  this->last_send_ = now;
  uint8_t buf[3] = {0xd2, 0x80, 0x52};
  if (this->buttons_.empty() || this->sending_) {
    this->sending_ = false;
    this->control_uart_->write_array(buf, 3);
    buf[1] = 0;
    buf[2] = 0xd2;
    for (int i = 0; i < 4; i++)
      this->control_uart_->write_array(buf, 3);
    return;
  }
  this->sending_ = true;
  uint8_t button = this->buttons_[0];
  this->buttons_.erase(this->control_buffer_.begin());
  buf[1] += button;
  buf[2] = 0xd2 + buf[1];
  this->control_uart_->write_array(buf, 3);
  buf[1] = button;
  buf[2] = 0xd2 + button;
  for (int i = 0; i < 4; i++)
    this->control_uart_->write_array(buf, 3);
}

void PuroAir::dump_config() {
  ESP_LOGCONFIG(TAG, "PuroAir air purifier");
}

}  // namespace puroair
}  // namespace esphome
