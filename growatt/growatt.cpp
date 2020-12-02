#include "growatt.h"
#include "esphome/core/log.h"

namespace esphome {
namespace growatt {

static const char *TAG = "growatt";

static const uint8_t CMD_READ_INPUT_REG = 0x03;
static const uint8_t REGISTER_COUNT = 42;

void Growatt::on_modbus_data(const std::vector<uint8_t> &data) {
  if (data.size() < 84) {
    ESP_LOGW(TAG, "Invalid data packet size");
    return;
  }

  auto get_16bit = [&](int i) -> uint16_t {
    return (uint16_t(data[i]) << 8) | uint16_t(data[i + 1]);
  };
  auto get_32bit = [&](int i) -> uint32_t {
    return (uint32_t(get_16bit(i + 2)) << 16) | uint32_t(get_16bit(i));
  };

  uint32_t raw_32 = get_32bit(1);
  float input_power = raw_32 / 10.0f;

  uint16_t raw_16 = get_16bit(3);
  float pv1_voltage = raw_16 / 10.0f;
  raw_16 = get_16bit(4);
  float pv1_current = raw_16 / 10.0f;
  raw_32 = get_32bit(5);
  float pv1_power = raw_32 / 10.0f;

  raw_16 = get_16bit(7);
  float pv2_voltage = raw_16 / 10.0f;
  raw_16 = get_16bit(8);
  float pv2_current = raw_16 / 10.0f;
  raw_32 = get_32bit(9);
  float pv2_power = raw_32 / 10.0f;

  raw_32 = get_32bit(11);
  float output_power = raw_32 / 10.0f;

  raw_16 = get_16bit(13);
  float grid_frequency = raw_16 / 100.0f;

  raw_16 = get_16bit(32);
  float temperature = raw_16 / 10.0f;

  ESP_LOGD(TAG, "DATA: IP=%.1fW, V1=%.1fV, I1=%.3fA, P1=%.1fW, V2=%.1fV, I2=%.3fA, P2=%.1fW, OP=%.1fW, F=%.1f Hz, TEMP=%.2f",
           input_power, pv1_voltage, pv1_current, pv1_power, pv2_voltage, pv2_current, pv2_power, output_power,
           grid_frequency, temperature);
  if (this->input_power_sensor_ != nullptr)
    this->input_power_sensor_->publish_state(input_power);
  if (this->pv1_voltage_sensor_ != nullptr)
    this->pv1_voltage_sensor_->publish_state(pv1_voltage);
  if (this->pv1_current_sensor_ != nullptr)
    this->pv1_current_sensor_->publish_state(pv1_current);
  if (this->pv1_power_sensor_ != nullptr)
    this->pv1_power_sensor_->publish_state(pv1_power);
  if (this->pv2_voltage_sensor_ != nullptr)
    this->pv2_voltage_sensor_->publish_state(pv2_voltage);
  if (this->pv2_current_sensor_ != nullptr)
    this->pv2_current_sensor_->publish_state(pv2_current);
  if (this->pv2_power_sensor_ != nullptr)
    this->pv2_power_sensor_->publish_state(pv2_power);
  if (this->output_power_sensor_ != nullptr)
    this->output_power_sensor_->publish_state(output_power);
  if (this->grid_frequency_sensor_ != nullptr)
    this->grid_frequency_sensor_->publish_state(grid_frequency);
  if (this->output_power_sensor_ != nullptr)
    this->output_power_sensor_->publish_state(output_power);
  if (this->temperature_sensor_ != nullptr)
    this->temperature_sensor_->publish_state(temperature);
}

void Growatt::update() { this->send(CMD_READ_INPUT_REG, 0, REGISTER_COUNT); }

void Growatt::dump_config() {
  ESP_LOGCONFIG(TAG, "Growatt:");
  ESP_LOGCONFIG(TAG, "  Address: 0x%02X", this->address_);
  LOG_SENSOR("", "Input Power", this->input_power_sensor_);
  LOG_SENSOR("", "PV1 Voltage", this->pv1_voltage_sensor_);
  LOG_SENSOR("", "PV1 Current", this->pv1_current_sensor_);
  LOG_SENSOR("", "PV1 Power", this->pv1_power_sensor_);
  LOG_SENSOR("", "PV2 Voltage", this->pv2_voltage_sensor_);
  LOG_SENSOR("", "PV2 Current", this->pv2_current_sensor_);
  LOG_SENSOR("", "PV2 Power", this->pv2_power_sensor_);
  LOG_SENSOR("", "Grid Frequency", this->grid_frequency_sensor_);
  LOG_SENSOR("", "Output Power", this->output_power_sensor_);
  LOG_SENSOR("", "Temperature", this->temperature_sensor_);
}

}  // namespace growatt
}  // namespace esphome
