#include "gfsun2000.h"
#include "esphome/core/log.h"

namespace esphome {
namespace gfsun2000 {

static const char *TAG = "gfsun2000";

static const uint8_t CMD_READ_INPUT_REG = 0x03;
static const uint8_t REGISTER_START[] = {0, 3, 60, 70, 86, 109};
static const uint8_t REGISTER_COUNT[] = {0, 5, 5, 1, 1, 1};

void GFSun2000::on_modbus_data(const std::vector<uint8_t> &data) {
  uint32_t raw_32;
  int32_t sraw_32;
  uint16_t raw_16;
  int16_t sraw_16;

  auto get_u16bit = [&](int i) -> uint16_t {
    return (data[i * 2] << 8) | data[i * 2 + 1];
  };
  auto get_s16bit = [&](int i) -> int16_t {
    return (int8_t(data[i * 2]) << 8) | data[i * 2 + 1];
  };
  auto get_u32bit = [&](int i) -> int32_t {
    return (get_u16bit(i + 1) << 16) | get_u16bit(i);
  };
  auto get_s32bit = [&](int i) -> int32_t {
    return (get_s16bit(i + 1) << 16) | get_u16bit(i);
  };

  waiting_ = false;
  ESP_LOGD(TAG, "Data: %s", hexencode(data).c_str());
  if (data.size() < REGISTER_COUNT[state_] * 2) {
    ESP_LOGW(TAG, "Invalid data packet size (%d) for state %d", data.size(), state_);
    return;
  }

  if (state_ == 1) {
    state_ = 2;
    std::string s(data.begin(), data.end());
    ESP_LOGD(TAG, "DATA: Device ID='%s'", s.c_str());
    if (device_id_sensor_ != nullptr)
      device_id_sensor_->publish_state(s);
    return;
  }
  if (state_ == 2) {
    state_ = 3;
    sraw_16 = get_s16bit(0); // register 60
    float custom_energy = sraw_16 * 100.0f;
    sraw_32 = get_s32bit(3); // register 63
    float total_energy = sraw_32 * 100.0f;
    ESP_LOGD(TAG, "DATA: Custom Energy=%0.1fKWh, Total Energy=%0.1fKWh", custom_energy / 1000, total_energy / 1000);
    if (custom_energy_sensor_ != nullptr)
      custom_energy_sensor_->publish_state(custom_energy);
    if (total_energy_sensor_ != nullptr)
      total_energy_sensor_->publish_state(total_energy);
    return;
  }
  if (state_ == 3) {
    state_ = 4;
    raw_16 = get_u16bit(0); // register 70
    float ac_voltage = raw_16 / 10.0f;
    ESP_LOGD(TAG, "DATA: AC Voltage=%0.1f", ac_voltage);
    if (ac_voltage_sensor_ != nullptr)
      ac_voltage_sensor_->publish_state(ac_voltage);
    return;
  }
  if (state_ == 4) {
    state_ = 5;
    raw_16 = get_u16bit(0); // register 86
    float output_power = raw_16 / 10.0f;
    ESP_LOGD(TAG, "DATA: Output Power=%0.1fW", output_power);
    if (output_power_sensor_ != nullptr)
      output_power_sensor_->publish_state(output_power);
    return;
  }
  if (state_ == 5) {
    state_ = 0;
    raw_16 = get_u16bit(0); // register 109
    float dc_voltage = raw_16 / 10.0f;
    ESP_LOGD(TAG, "DATA: DC Voltage=%0.1f", dc_voltage);
    if (dc_voltage_sensor_ != nullptr)
      dc_voltage_sensor_->publish_state(dc_voltage);
    return;
  }

}

void GFSun2000::loop() {
  long now = millis();
  // timeout after 15 seconds
  if (waiting_ && (now - last_send_ > 5000)) {
    ESP_LOGW(TAG, "timed out waiting for response");
    waiting_ = false;
  }
  if (waiting_ || (state_ == 0) || (now - last_send_ < 1000))
    return;
  last_send_ = now;
  send(CMD_READ_INPUT_REG, REGISTER_START[state_], REGISTER_COUNT[state_]);
  waiting_ = true;
}

void GFSun2000::update() {
  static bool first = true;
  if (first) {
    state_ = 1;
    first = false;
    return;
  }
  state_ = 2;
}


void GFSun2000::dump_config() {
  ESP_LOGCONFIG(TAG, "GFSun2000:");
  ESP_LOGCONFIG(TAG, "  Address: 0x%02X", address_);
  LOG_TEXT_SENSOR("", "Device ID", device_id_sensor_);
  LOG_SENSOR("", "AC Voltage", ac_voltage_sensor_);
  LOG_SENSOR("", "DC Current", dc_voltage_sensor_);
  LOG_SENSOR("", "Output Power", output_power_sensor_);
  LOG_SENSOR("", "Custom Energy", custom_energy_sensor_);
  LOG_SENSOR("", "Total Energy", total_energy_sensor_);
}

}  // namespace gfsun2000
}  // namespace esphome
