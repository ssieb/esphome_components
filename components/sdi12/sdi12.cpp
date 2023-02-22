#include "sdi12.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace sdi12 {

static const char *const TAG = "sdi12";

void SDI12::dump_config() {
  ESP_LOGCONFIG(TAG, "SDI12:");
  check_uart_settings(1200);
}

static uint8_t addr2chr(uint8_t addr) {
  if (addr <= 9)
    return addr + '0';
  if (addr <= 35)
    return addr + 'a' - 10;
  if (addr <= 61)
    return addr + 'A' - 36;
  return ':';
}

static uint8_t chr2addr(uint8_t chr) {
  if ((chr >= '0') && (chr <= '9'))
    return chr - '0';
  if ((chr >= 'a') && (chr <= 'z'))
    return chr - 'a' + 10;
  if ((chr >= 'A') && (chr <= 'Z'))
    return chr - 'A' + 36;
  return 0xff;
}

void SDI12::setup() {
  this->scanning_ = false;
  this->cur_addr_ = 0;
}

void SDI12::loop() {
  uint32_t now = millis();
  if (!this->available()) {
    if (this->waiting_) {
      if (now - this->start_ < this->timeout_)
        return;
      this->waiting_ = false;
    }
    if (this->scanning_) {
      if (++this->tries_ >= 2) {
        this->tries_ = 0;
        if (++this->cur_addr_ > 61) {
          this->scanning_ = false;
          return;
        }
      }
      this->start_ = now;
      this->timeout_ = 30;
      this->buffer_.clear();
      uint8_t cmd[2];
      cmd[0] = addr2chr(this->cur_addr_);
      cmd[1] = '!';
      this->write_array(cmd, 2);
    }
    return;
  }

  while (available()) {
    uint8_t c;
    read_byte(&c);

    if (this->state_ == 0) {
      if (c == 0)
        continue;
      this->address_ = chr2addr(c);
      this->state_ = 1;
      this->buffer_.clear();
      continue;
    }

    if (c == '\r')
      continue;
    if (c != '\n') {
      if (this->buffer_.size() < 35)
        this->buffer_.push_back(c);
      continue;
    }
    this->state_ = 0;

    this->buffer_.push_back(0);
    uint8_t *data = this->buffer_.data();
    ESP_LOGD(TAG, "%u(%c) sent: %s", this->address_, addr2chr(this->address_), data);

    if (this->phase_ == 0) {
      if (this->buffer_.size() != 5)
        ESP_LOGE(TAG, "unexpected response");
      int delay = (data[0] - '0') * 100 + (data[1] - '0') * 10 + (data[2] - '0');
      this->mexpect_ = data[3] - '0';
      ESP_LOGD(TAG, "%d values will be available in %d seconds", this->mexpect_, delay);
      this->phase_++;
      this->dataset_ = 0;
      this->values_.clear();
      if (delay > 0)
        continue;
      this->phase_++;
    }

    if (this->phase_ == 1) {
      if (this->buffer_.size() != 1)
        ESP_LOGE(TAG, "unexpected response");
      this->phase_++;
    }

    uint8_t buf[4] = { addr2chr(this->address_), 'D', '!', '0' };
    if (this->phase_ == 2) {
      this->write_array(buf, 4);
      this->phase_++;
      continue;
    }

    if (this->phase_ == 3) {
      char num[10];
      while (*data) {
        char *dest = num;
        *dest++ = *data++;
        while (*data && (*data != '+') && (*data != '-'))
          *dest++ = *data++;
        *dest = 0;
        this->values_.push_back(parse_number<float>(num).value());
      }
      if (this->values_.size() < this->mexpect_) {
        buf[3] += ++this->dataset_;
        this->write_array(buf, 4);
        continue;
      }
      for (auto &listener : this->listeners_)
        listener->on_values(this->address_, this->values_);
    }
  }
}

void SDI12::start_measurement(uint8_t address) {
  this->phase_ = 0;
  this->state_ = 0;
  this->buffer_.clear();
  uint8_t buf[3] = { addr2chr(address), 'M', '!' };
  this->write_array(buf, 3);
}

void SDI12Listener::on_values(uint8_t address, std::vector<float> &values) {
  if (this->address_ != address)
    return;
  this->handle_values(values);
}

}  // namespace sdi12
}  // namespace esphome
