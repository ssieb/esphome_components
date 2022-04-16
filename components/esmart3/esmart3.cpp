#include "esmart3.h"
#include "esphome/core/log.h"

namespace esphome {
namespace esmart3 {

static const char *TAG = "esmart3";

void ESmart3Component::dump_config() {
  ESP_LOGCONFIG(TAG, "Esmart3:");
  LOG_SENSOR("  ", "Charge Mode", charge_mode_sensor_);
  LOG_SENSOR("  ", "Input Voltage", input_voltage_sensor_);
  LOG_SENSOR("  ", "Battery Voltage", battery_voltage_sensor_);
  LOG_SENSOR("  ", "Charging Current", charging_current_sensor_);
  LOG_SENSOR("  ", "Load Voltage", load_voltage_sensor_);
  LOG_SENSOR("  ", "Load Current", load_current_sensor_);
  LOG_SENSOR("  ", "Charging Power", charging_power_sensor_);
  LOG_SENSOR("  ", "Load Power", load_power_sensor_);
  LOG_SENSOR("  ", "Battery Temp", battery_temp_sensor_);
  LOG_SENSOR("  ", "Internal Temp", internal_temp_sensor_);
  LOG_SENSOR("  ", "Battery Level", battery_level_sensor_);
  this->check_uart_settings(9600);
}

void ESmart3Component::update() {
  if (receiving_) {
    ESP_LOGW(TAG, "update interval overrun");
    return;
  }
  static uint8_t data[] = {0xaa, 1, 0, 1, 0, 3, 0, 0, 24, 0x39};
  write_array(data, 10);
}

void ESmart3Component::loop() {
  const uint32_t now = millis();
  if (receiving_ && (now - last_transmission_ >= 500)) {
    // last transmission too long ago. Reset RX index.
    ESP_LOGW(TAG, "Last transmission too long ago. Reset RX index.");
    data_.clear();
    receiving_ = false;
  }

  if (!available())
    return;

  last_transmission_ = now;
  while (available()) {
    uint8_t c;
    read_byte(&c);
    if (!receiving_) {
      if (c != 0xaa)
        continue;
      receiving_ = true;
    }
    data_.push_back(c);
    if (data_.size() == 6)
      data_count_ = c;
    if ((data_.size() > 6) and (data_.size() == data_count_ + 7)) {
      if (check_data_())
        parse_data_();
      data_.clear();
      receiving_ = false;
    }
  }
}

bool ESmart3Component::check_data_() const {
  if (data_[3] != 3) {
    ESP_LOGW(TAG, "unexpected response code: %d", data_[3]);
    return false;
  }
  uint8_t sum = 0;
  for (uint8_t c: data_)
    sum += c;
  bool result = sum == 0;
  if (!result)
    ESP_LOGW(TAG, "data checksum failed");
  return result;
}

void ESmart3Component::parse_data_() {
  uint16_t charge_mode = get_16_bit_uint_(8);
  float input_voltage = float(get_16_bit_uint_(10)) / 10;
  float battery_voltage = float(get_16_bit_uint_(12)) / 10;
  float charging_current = float(get_16_bit_uint_(14)) / 10;
  float load_voltage = float(get_16_bit_uint_(18)) / 10;
  float load_current = float(get_16_bit_uint_(20)) / 10;
  uint16_t charging_power = get_16_bit_uint_(22);
  uint16_t load_power = get_16_bit_uint_(24);
  uint16_t battery_temp = get_16_bit_uint_(26);
  uint16_t internal_temp = get_16_bit_uint_(28);
  uint16_t battery_level = get_16_bit_uint_(30);

  ESP_LOGD(TAG, "Got ChgMode: %d, PvVolt: %0.1fV, BatVolt: %0.1fV, ChgCurr: %0.1fA, LoadVolt: %0.1fV, LoadCurr: %0.1fV",
           charge_mode, input_voltage, battery_voltage, charging_current, load_voltage, load_current);
  ESP_LOGD(TAG, "Got ChgPower: %dW, LoadPower: %dW, BatTemp: %d°C, InnerTemp: %d°C, BatCap: %d%%",
           charging_power, load_power, battery_temp, internal_temp, battery_level);

  if (charge_mode_sensor_ != nullptr)
    charge_mode_sensor_->publish_state(charge_mode);
  if (input_voltage_sensor_ != nullptr)
    input_voltage_sensor_->publish_state(input_voltage);
  if (battery_voltage_sensor_ != nullptr)
    battery_voltage_sensor_->publish_state(battery_voltage);
  if (charging_current_sensor_ != nullptr)
    charging_current_sensor_->publish_state(charging_current);
  if (load_voltage_sensor_ != nullptr)
    load_voltage_sensor_->publish_state(load_voltage);
  if (load_current_sensor_ != nullptr)
    load_current_sensor_->publish_state(load_current);
  if (charging_power_sensor_ != nullptr)
    charging_power_sensor_->publish_state(charging_power);
  if (load_power_sensor_ != nullptr)
    load_power_sensor_->publish_state(load_power);
  if (battery_temp_sensor_ != nullptr)
    battery_temp_sensor_->publish_state(battery_temp);
  if (internal_temp_sensor_ != nullptr)
    internal_temp_sensor_->publish_state(internal_temp);
  if (battery_level_sensor_ != nullptr)
    battery_level_sensor_->publish_state(battery_level);
}

uint16_t ESmart3Component::get_16_bit_uint_(uint8_t start_index) const {
  return (uint16_t(this->data_[start_index + 1]) << 8) | uint16_t(this->data_[start_index]);
}

}  // namespace esmart3
}  // namespace esphome
