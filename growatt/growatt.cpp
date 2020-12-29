#include "growatt.h"
#include "esphome/core/log.h"

namespace esphome {
namespace growatt {

static const char *TAG = "growatt";

static const uint8_t CMD_READ_INPUT_REG = 0x04;
static const uint16_t REGISTER_START[] = {0, 93, 1048};
static const uint16_t REGISTER_COUNT[] = {57, 1, 4};

void Growatt::on_modbus_data(const std::vector<uint8_t> &data) {
  uint32_t raw_32;
  uint16_t raw_16;

  auto get_16bit = [&](int i) -> uint16_t {
    return (uint16_t(data[i * 2]) << 8) | uint16_t(data[i * 2 + 1]);
  };
  auto get_32bit = [&](int i) -> uint32_t {
    return (uint32_t(get_16bit(i)) << 16) | uint32_t(get_16bit(i + 1));
  };

  this->waiting_ = false;
  if (data.size() < REGISTER_COUNT[this->state_ - 1] * 2) {
    ESP_LOGW(TAG, "Invalid data packet size (%d) for state %d", data.size(), this->state_);
    return;
  }
  ESP_LOGD(TAG, "Data: %s", hexencode(data).c_str());

  if (this->state_ == 1) {
    this->state_ = 2;
    raw_32 = get_32bit(1);
    float input_power = raw_32 / 10.0f;

    raw_16 = get_16bit(3);
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

    raw_32 = get_32bit(35);
    float output_power = raw_32 / 10.0f;

    raw_16 = get_16bit(37);
    float grid_frequency = raw_16 / 100.0f;

    raw_16 = get_16bit(38);
    float ac_voltage = raw_16 / 10.0f;
    raw_16 = get_16bit(39);
    float ac_current = raw_16 / 10.0f;
    raw_32 = get_32bit(40);
    float ac_power = raw_32 / 10.0f;

    raw_32 = get_32bit(53);
    float today_gen = raw_32 / 10.0f;
    raw_32 = get_32bit(55);
    float total_gen = raw_32 / 10.0f;

    ESP_LOGD(TAG, "DATA: IP=%.1fW V1=%.1fV I1=%.3fA P1=%.1fW V2=%.1fV I2=%.3fA P2=%.1fW OP=%.1fW F=%.1fHz "
             "VAC=%.1fV IAC=%.3fA PAC=%.1fW TDGE=%.1fkWh TTGE=%.1fkWh",
             input_power, pv1_voltage, pv1_current, pv1_power, pv2_voltage, pv2_current, pv2_power, output_power, grid_frequency,
             ac_voltage, ac_current, ac_power, today_gen, total_gen);
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
    if (this->ac_voltage_sensor_ != nullptr)
      this->ac_voltage_sensor_->publish_state(ac_voltage);
    if (this->ac_current_sensor_ != nullptr)
      this->ac_current_sensor_->publish_state(ac_current);
    if (this->ac_power_sensor_ != nullptr)
      this->ac_power_sensor_->publish_state(ac_power);
    if (this->today_gen_sensor_ != nullptr)
      this->today_gen_sensor_->publish_state(today_gen);
    if (this->total_gen_sensor_ != nullptr)
      this->total_gen_sensor_->publish_state(total_gen);
    return;
  }
  if (this->state_ == 2) {
    this->state_ = 3;
    raw_16 = get_16bit(0); // register 93
    float temperature = raw_16 / 10.0f;
    ESP_LOGD(TAG, "DATA: TEMP=%.2f", temperature);
    if (this->temperature_sensor_ != nullptr)
      this->temperature_sensor_->publish_state(temperature);
    return;
  }
  if (this->state_ == 3) {
    this->state_ = 0;
    raw_32 = get_32bit(0); // register 1048
    float today_grid = raw_32 / 10.0f;
    raw_32 = get_32bit(2);
    float total_grid = raw_32 / 10.0f;
    ESP_LOGD(TAG, "DATA: TDGE=%.1fkWh TTGE=%.1fkWh", today_grid, total_grid);
    if (this->today_grid_sensor_ != nullptr)
      this->today_grid_sensor_->publish_state(today_grid);
    if (this->total_grid_sensor_ != nullptr)
      this->total_grid_sensor_->publish_state(total_grid);
    return;
  }
}

void Growatt::loop() {
  long now = millis();
  // timeout after 15 seconds
  if (this->waiting_ && (now - this->last_send > 15000)) {
    ESP_LOGW(TAG, "timed out waiting for response");
    this->waiting_ = false;
  }
  if (this->waiting_ || (this->state_ == 0) || (now - this->last_send_ < 1000))
    return;
  this->last_send_ = now;
  this->send(CMD_READ_INPUT_REG, REGISTER_START[this->state_ - 1], REGISTER_COUNT[this->state_ - 1]);
  this->waiting_ = true;
}

void Growatt::update() { this->state_ = 1; }

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
  LOG_SENSOR("", "Output Power", this->output_power_sensor_);
  LOG_SENSOR("", "Grid Frequency", this->grid_frequency_sensor_);
  LOG_SENSOR("", "AC Voltage", this->ac_voltage_sensor_);
  LOG_SENSOR("", "AC Current", this->ac_current_sensor_);
  LOG_SENSOR("", "AC Power", this->ac_power_sensor_);
  LOG_SENSOR("", "Today Generated", this->today_gen_sensor_);
  LOG_SENSOR("", "Total Generated", this->total_gen_sensor_);
  LOG_SENSOR("", "Temperature", this->temperature_sensor_);
  LOG_SENSOR("", "Today to Grid", this->today_grid_sensor_);
  LOG_SENSOR("", "Total to Grid", this->total_grid_sensor_);
}

}  // namespace growatt
}  // namespace esphome
