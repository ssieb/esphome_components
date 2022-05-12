#include "jbd_bms.h"
#include "esphome/core/log.h"

namespace esphome {
namespace jbd_bms {

static const char *TAG = "jbd_bms";
static const uint8_t JBD_BMS_CMD_BASIC = 3;
static const uint8_t JBD_BMS_CMD_VOLTAGE = 4;
static const uint8_t JBD_BMS_CMD_VERSION = 5;

void JBD_BMS::dump_config() {
  ESP_LOGCONFIG(TAG, "JBD BMS:");
  if (this->version_.size() > 0)
    ESP_LOGCONFIG(TAG, "  Version: %s", this->version_.c_str());
  if (this->string_count_ > 0)
    ESP_LOGCONFIG(TAG, "  %d battery strings", this->string_count_);
  for (auto nsensor: this->battery_voltage_sensors_) {
    std::string s = str_sprintf("String %d Voltage", nsensor.num + 1);
    LOG_SENSOR("  ", "String Voltage", nsensor.sensor);
  }
  if (this->ntc_count_ > 0)
    ESP_LOGCONFIG(TAG, "  %d NTC sensors", this->ntc_count_);
  for (auto nsensor: this->temperature_sensors_) {
    std::string s = str_sprintf("NTC %d", nsensor.num);
    LOG_SENSOR("  ", "NTC", nsensor.sensor);
  }
  LOG_SENSOR("  ", "Voltage", this->voltage_sensor_);
  LOG_SENSOR("  ", "Current", this->current_sensor_);
  LOG_SENSOR("  ", "Balance Capacity", this->balance_capacity_sensor_);
  LOG_SENSOR("  ", "Rate Capacity", this->rate_capacity_sensor_);
  LOG_SENSOR("  ", "Capacity", this->capacity_sensor_);
  LOG_BINARY_SENSOR("  ", "Charge FET", this->charge_sensor_);
  LOG_BINARY_SENSOR("  ", "Discharge FET", this->discharge_sensor_);
  LOG_BINARY_SENSOR("  ", "COVP", this->prot_covp_sensor_);
  LOG_BINARY_SENSOR("  ", "CUVP", this->prot_cuvp_sensor_);
  LOG_BINARY_SENSOR("  ", "POVP", this->prot_povp_sensor_);
  LOG_BINARY_SENSOR("  ", "PUVP", this->prot_puvp_sensor_);
  LOG_BINARY_SENSOR("  ", "CHGOT", this->prot_chgot_sensor_);
  LOG_BINARY_SENSOR("  ", "CHGUT", this->prot_chgut_sensor_);
  LOG_BINARY_SENSOR("  ", "DSGOT", this->prot_dsgot_sensor_);
  LOG_BINARY_SENSOR("  ", "DSGUT", this->prot_dsgut_sensor_);
  LOG_BINARY_SENSOR("  ", "CHGOC", this->prot_chgoc_sensor_);
  LOG_BINARY_SENSOR("  ", "DSGOC", this->prot_dsgoc_sensor_);
  LOG_BINARY_SENSOR("  ", "AFE", this->prot_afe_sensor_);
  LOG_BINARY_SENSOR("  ", "SWLOCK", this->swlock_sensor_);
  this->check_uart_settings(9600);
}

void JBD_BMS::update() {
  if (this->update_) {
    ESP_LOGW(TAG, "update interval overrun");
    return;
  }
  this->update_ = true;
}

void JBD_BMS::loop() {
  static bool receiving = false;
  static int state = 0;
  static uint32_t last_data = 0;
  const uint32_t now = millis();

  if (state == 0) {
    while (available())
      this->read();
    if (!this->update_)
      return;
    state = 1;
    this->update_ = false;
  }

  if (receiving) {
    if (now - last_data >= 250) {
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
   case 1: cmd = JBD_BMS_CMD_BASIC; break;
   case 2: cmd = JBD_BMS_CMD_VOLTAGE; break;
   case 3: cmd = JBD_BMS_CMD_VERSION; break;
  }
  uint8_t buf[7] = {0xdd, 0xa5, cmd, 0, 0xff, (uint8_t)-cmd, 0x77};
  this->write_array(buf, 7);
  receiving = true;
  this->read_data_(true);
  last_data = now;
}

bool JBD_BMS::read_data_(bool first) {
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
        state++;
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
      dcsum += c;
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

void JBD_BMS::parse_data_(uint8_t cmd) {
  if (cmd == 3) {
    if (this->voltage_sensor_ != nullptr)
      this->voltage_sensor_->publish_state(get_16_bit_uint_(0) / 100.0f);
    if (this->current_sensor_ != nullptr)
      this->current_sensor_->publish_state(((int16_t)get_16_bit_uint_(2)) / 100.0f);
    if (this->balance_capacity_sensor_ != nullptr)
      this->balance_capacity_sensor_->publish_state(get_16_bit_uint_(4) / 100.0f);
    if (this->rate_capacity_sensor_ != nullptr)
      this->rate_capacity_sensor_->publish_state(get_16_bit_uint_(6) / 100.0f);
    uint32_t bits = (get_16_bit_uint_(14) << 16) + get_16_bit_uint_(12);
    for (auto nsensor: this->balance_state_sensors_)
      nsensor.sensor->publish_state(bits & (1 << nsensor.num));
    if (this->prot_covp_sensor_ != nullptr)
      this->prot_covp_sensor_->publish_state(this->data_[17] & 1);
    if (this->prot_cuvp_sensor_ != nullptr)
      this->prot_cuvp_sensor_->publish_state(this->data_[17] & 2);
    if (this->prot_povp_sensor_ != nullptr)
      this->prot_povp_sensor_->publish_state(this->data_[17] & 4);
    if (this->prot_puvp_sensor_ != nullptr)
      this->prot_puvp_sensor_->publish_state(this->data_[17] & 8);
    if (this->prot_chgot_sensor_ != nullptr)
      this->prot_chgot_sensor_->publish_state(this->data_[17] & 0x10);
    if (this->prot_chgut_sensor_ != nullptr)
      this->prot_chgut_sensor_->publish_state(this->data_[17] & 0x20);
    if (this->prot_dsgot_sensor_ != nullptr)
      this->prot_dsgot_sensor_->publish_state(this->data_[17] & 0x40);
    if (this->prot_dsgut_sensor_ != nullptr)
      this->prot_dsgut_sensor_->publish_state(this->data_[17] & 0x80);
    if (this->prot_chgoc_sensor_ != nullptr)
      this->prot_chgoc_sensor_->publish_state(this->data_[16] & 1);
    if (this->prot_dsgoc_sensor_ != nullptr)
      this->prot_dsgoc_sensor_->publish_state(this->data_[16] & 2);
    if (this->prot_short_sensor_ != nullptr)
      this->prot_short_sensor_->publish_state(this->data_[16] & 4);
    if (this->prot_afe_sensor_ != nullptr)
      this->prot_afe_sensor_->publish_state(this->data_[16] & 8);
    if (this->swlock_sensor_ != nullptr)
      this->swlock_sensor_->publish_state(this->data_[16] & 0x10);
    if (this->capacity_sensor_ != nullptr)
      this->capacity_sensor_->publish_state(this->data_[19]);
    if (this->charge_sensor_ != nullptr)
      this->charge_sensor_->publish_state(this->data_[20] & 1);
    if (this->discharge_sensor_ != nullptr)
      this->discharge_sensor_->publish_state(this->data_[20] & 2);
    if (this->string_count_ == 0) {
      this->string_count_ = this->data_[21];
      ESP_LOGD(TAG, "%d battery strings", this->string_count_);
    }
    int count = this->data_[22];
    if (this->ntc_count_ == 0) {
      this->ntc_count_ = count;
      ESP_LOGD(TAG, "%d NTC sensors", this->ntc_count_);
    }
    for (auto nsensor: this->temperature_sensors_) {
      if (nsensor.num < count) {
        int offset = nsensor.num * 2 + 23;
        nsensor.sensor->publish_state((get_16_bit_uint_(offset) - 2731) / 10.0f);
      }
    }
  } else if (cmd == 4) {
    int count = this->data_.size() / 2;
    for (auto nsensor: this->battery_voltage_sensors_) {
      if (nsensor.num < count) {
        int offset = nsensor.num * 2;
        nsensor.sensor->publish_state(get_16_bit_uint_(offset) / 1000.0f);
      }
    }
  } else if (cmd == 5) {
    this->version_.assign((char *)this->data_.data(), this->data_.size());
    ESP_LOGD(TAG, "version: %s", this->version_.c_str());
  } else {
    ESP_LOGW(TAG, "unknown command %02x", cmd);
  }
}

uint16_t JBD_BMS::get_16_bit_uint_(uint8_t start_index) {
  return (uint16_t(this->data_[start_index]) << 8) | uint16_t(this->data_[start_index + 1]);
}

}  // namespace jbd_bms
}  // namespace esphome
