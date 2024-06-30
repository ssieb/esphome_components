#include "u_inverter.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include <cinttypes>

namespace esphome {
namespace u_inverter {

static const char *const TAG = "u_inverter";

void UInverter::dump_config() {
  ESP_LOGCONFIG(TAG, "Unknown Inverter:");
  ESP_LOGCONFIG(TAG, "  Version: %s", this->version_.c_str());
  check_uart_settings(2400);
}

void UInverter::setup() {
  this->send_command(CMD_HIMSG1);
}

void UInverter::update() {
  this->send_command(CMD_HSTS);
  this->send_command(CMD_HGRID);
  this->send_command(CMD_HOP);
  this->send_command(CMD_HBAT);
  this->send_command(CMD_HPV);
}

void UInverter::send_command(UInverterCmd cmd) {
  if (this->queue_.size() >= 20) {
    ESP_LOGE(TAG, "command queue overflow");
    return;
  }
  this->queue_.push_back(cmd);
  if (this->state_ == 0)
    this->check_queue_();
}

static const char *cmd_to_str(UInverterCmd cmd) {
  switch (cmd) {
    case CMD_HSTS: return "HSTS";
    case CMD_HIMSG1: return "HIMSG1";
    case CMD_HGRID: return "HGRID";
    case CMD_HOP: return "HOP";
    case CMD_HBAT: return "HBAT";
    case CMD_HPV: return "HPV";
    case CMD_HTEMP: return "HTEMP";
    case CMD_HEEP: return "HEEP";
    case CMD_HBMS: return "HBMS";
    case CMD_HGEN: return "HGEN";
  }
  return nullptr;
}

void UInverter::loop() {
  uint32_t now = millis();
  if (!this->available()) {
    if (this->state_ != 0) {
      if (now - this->last_data_time_ < 1000)
        return;
      const char *cmd_str = cmd_to_str(this->queue_[0]);
      if (cmd_str == nullptr)
        cmd_str = "unknown";
      ESP_LOGE(TAG, "%s request timed out", cmd_str);
    }
    this->check_queue_();
    return;
  }
  this->last_data_time_ = now;

  while (this->available()) {
    uint8_t c;
    this->read_byte(&c);

    switch (this->state_) {
      case 0:
        break;
      case 1:
        if (c != '(')
          continue;
        this->state_ = 2;
        break;
      case 2:
        if (c == '\r') {
          this->handle_response_();
        } else if (this->buffer_.size() >= 100) {
          this->state_ = 3;
        } else {
          this->buffer_.push_back(c);
        }
        break;
      case 3:
        if (c == '\r') {
          this->check_queue_();
        }
        break;
    }
  }
}

void UInverter::check_queue_() {
  this->state_ = 0;
  this->buffer_.clear();
  while (!this->queue_.empty()) {
    UInverterCmd cmd = this->queue_[0];
    const char *cmd_str = cmd_to_str(cmd);
    if (cmd_str == nullptr) {
      ESP_LOGE(TAG, "invalid command %d in queue", cmd);
      this->queue_.erase(this->queue_.begin());
      continue;
    }
    ESP_LOGV(TAG, "sending %s command", cmd_str);
    this->write_str(cmd_str);
    this->write_byte('\r');
    this->state_ = 1;
    this->last_data_time_ = millis();
    return;
  }
}

void UInverter::handle_response_() {
  UInverterCmd cmd = this->queue_[0];
  this->queue_.erase(this->queue_.begin());
  std::string data = std::string(this->buffer_.begin(), this->buffer_.end());
  ESP_LOGV(TAG, "received '%s'", data.c_str());
  if (cmd == CMD_HIMSG1)
    this->version_ = data;
  for (auto &listener : this->listeners_)
    listener->handle_message(cmd, data);
  this->check_queue_();
}

}  // namespace u_inverter
}  // namespace esphome
