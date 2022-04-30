#include "overkill.h"
#include "esphome/core/log.h"

namespace esphome {
namespace overkill {

static const char *TAG = "overkill";
static const uint8_t OVERKILL_CMD_BASIC = 3;
static const uint8_t OVERKILL_CMD_VOLTAGE = 4;
static const uint8_t OVERKILL_CMD_VERSION = 5;

void Overkill::dump_config() {
  ESP_LOGCONFIG(TAG, "Overkill BMS:");
  if (this->version.size() > 0)
    ESP_LOGCONFIG(TAG, "  Version: %s", this->version.c_str());
  if (this->string_count_ > 0)
    ESP_LOGCONFIG(TAG, "  %d battery strings", this->string_count_);
  for (auto nsensor: this->battery_voltage_sensors_) {
    std::string s = str_sprintf("String %d Voltage", nsensor->num + 1);
    LOG_SENSOR("  ", s.c_str(), nsensor->sensor);
  }
  if (this->ntc_count_ > 0)
    ESP_LOGCONFIG(TAG, "  %d NTC sensors", this->ntc_count_);
  for (auto nsensor: this->temperature_sensors_) {
    std::string s = str_sprintf("NTC %d", nsensor->num);
    LOG_SENSOR("  ", s.c_str(), nsensor->sensor);
  }
  LOG_SENSOR("  ", "Voltage", this->voltage_sensor_);
  LOG_SENSOR("  ", "Current", this->current_sensor_);
  LOG_SENSOR("  ", "Balance Capacity", this->balance_capacity_sensor_);
  LOG_SENSOR("  ", "Rate Capacity", this->rate_capacity_sensor_);
  LOG_SENSOR("  ", "Capacity", this->capacity_sensor_);
  this->check_uart_settings(9600);
}

void Overkill::update() {
  if (this->update_) {
    ESP_LOGW(TAG, "update interval overrun");
    return;
  }
  this->update_ = true;
}

void Overkill::loop() {
  static bool receiving = false;
  static int state = 0;
  static uint32_t last_data = 0;
  const uint32_t now = millis();

  if (state == 0) {
    while (available())
      this->read();
    if (this->update_)
      state = 1;
    else
      return;
  }

  if (receiving) {
    if (now - last_data >= 100) {
      ESP_LOGW(TAG, "receive timeout");
      receiving = false;
    }
    if (!this->available())
      return;
    last_data = now;
    if (!this->read_data_())
      return;
    receiving = false;
    state++;
  }

  if (state > 2) {
    if ((state > 3) || (this->version_.size() > 0)) {
      state = 0;
      return;
    }
  }

  uint8_t cmd = 0;
  switch(state) {
   case 1: cmd = OVERKILL_CMD_BASIC; break;
   case 2: cmd = OVERKILL_CMD_VOLTAGE; break;
   case 3: cmd = OVERKILL_CMD_VERSION; break;
  }
  uint8_t buf[7] = {0xdd, 0xa5, cmd, 0, 0xff, -cmd, 0x77};
  this->write(buf, 7);
  receiving = true;
  this->read_data_(true);
  last_data = now;
}

bool Overkill::read_data_(bool first=false) {
  static int state;
  static uint16_t csum;
  static uint16_t dcsum;
  static uint8_t length;
  static uint8_t cmd;
  static bool error;

  if (first)
    state = 0;
  while (available()) {
    uint8_t c;
    read_byte(&c);
    switch(state) {
     case 0:
      if (c != 0xdd)
        continue;
      break;
     case 1:
      cmd = c;
      break;
     case 2:
      error = c & 0x80;
      csum = -c;
      break;
     case 3:
      length = c;
      if (length == 0)
        dstate++;
      this->data_.clear();
      csum -= c;
      break;
     case 4:
      this->data_.push_back(c);
      csum -= c;
      if (this->data_.size() < length)
        continue;
      break;
     case 5:
      dcsum = c << 8;
      break;
     case 6:
      dscum += c;
      break;
     case 7:
      if (c != 0x77) {
        ESP_LOGW(TAG, "incorrect end byte %02x received", c);
        state++;
        }
      if (dcsum != csum) {
        ESP_LOGW(TAG, "incorrect checksum received (%04x != %04x)", dcsum, csum);
        state++;
      }
      break;
    }
    if (state >= 7)
      break;
    state++;
  }
  if (state < 7)
    return false;
  if (error) {
    ESP_LOGW(TAG, "error signaled");
    return true;
  }
  if (state == 7)
    this->parse_data_(cmd);
  return true;
}

void Overkill::parse_data_(uint8_t cmd) {
  if (cmd == 3) {
    if (this->voltage_sensor_ != nullptr)
      this->voltage_sensor_.publish_state(get_16_bit_uint_(0) / 100.0f);
    if (this->current_sensor_ != nullptr)
      this->current_sensor_.publish_state(get_16_bit_uint_(2) / 100.0f);
    if (this->balance_capacity_sensor_ != nullptr)
      this->balance_capacity_sensor_.publish_state(get_16_bit_uint_(4) / 100.0f);
    if (this->rate_capacity_sensor_ != nullptr)
      this->rate_capacity_sensor_.publish_state(get_16_bit_uint_(6) / 100.0f);
    if (this->capacity_sensor_ != nullptr)
      this->capacity_sensor_.publish_state(this->data[19] << 8);
    if (this->string_count_ == 0) {
      this->string_count_ = this->data[21];
      ESP_LOGD(TAG, "%d battery strings", this->string_count_);
    }
    int count = this->data[22];
    if (this->ntc_count_ == 0) {
      this->ntc_count_ = count;
      ESP_LOGD(TAG, "%d NTC sensors", this->ntc_count_);
    }
    for (auto nsensor: this->temperature_sensors_) {
      if (nsensor->num < count) {
        int offset = nsensor->num * 2 + 23;
        nsensor->sensor.publish_state((get_16_bit_uint_(offset) - 2731) / 10.0f);
      }
    }
  } else if (cmd == 4) {
    int count = this->data_.size() / 2;
    for (auto nsensor: this->battery_voltage_sensors_) {
      if (nsensor->num < count) {
        int offset = nsensor->num * 2;
        nsensor->sensor.publish_state(get_16_bit_uint_(offset) / 1000.0f);
      }
    }
  } else if (cmd == 5) {
    this->data_.push_back(0);
    this->version_ = this->data_.data;
    ESP_LOGD(TAG, "version: %s", this->data_.data);
  } else {
    ESP_LOGW(TAG, "unknown command %02x", cmd);
  }
}

uint16_t Overkill::get_16_bit_uint_(uint8_t start_index) {
  return (uint16_t(this->data_[start_index]) << 8) | uint16_t(this->data_[start_index + 1]);
}

}  // namespace overkill
}  // namespace esphome
