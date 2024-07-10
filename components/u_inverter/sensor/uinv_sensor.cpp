#include "uinv_sensor.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace u_inverter {

static const char *const TAG = "u_inverter.sensor";

static inline uint16_t get_u16(std::vector<uint8_t> &message, int start) {
  return (message[start + 1] << 8) + message[start];
}

static inline int16_t get_i16(std::vector<uint8_t> &message, int start) {
  return (int16_t) ((message[start + 1] << 8) + message[start]);
}

void UInverterSensor::dump_config() {
  ESP_LOGCONFIG(TAG, "Unknown Inverter:");
  LOG_SENSOR("  ", "Fault Code", this->fault_code_sensor_);
  LOG_SENSOR("  ", "Mains Voltage", this->mains_voltage_sensor_);
  LOG_SENSOR("  ", "Mains Frequency", this->mains_frequency_sensor_);
  LOG_SENSOR("  ", "Mains Loss Voltage High", this->mains_lossv_high_sensor_);
  LOG_SENSOR("  ", "Mains Loss Voltage Low", this->mains_lossv_low_sensor_);
  LOG_SENSOR("  ", "Mains Loss Frequency High", this->mains_lossf_high_sensor_);
  LOG_SENSOR("  ", "Mains Loss Frequency Low", this->mains_lossf_low_sensor_);
  LOG_SENSOR("  ", "Mains Power", this->mains_power_sensor_);
  LOG_SENSOR("  ", "Output Voltage", this->output_voltage_sensor_);
  LOG_SENSOR("  ", "Output Frequency", this->output_frequency_sensor_);
  LOG_SENSOR("  ", "Output Reactive Power", this->output_reactive_power_sensor_);
  LOG_SENSOR("  ", "Output Active Power", this->output_active_power_sensor_);
  LOG_SENSOR("  ", "Load Percent", this->load_percent_sensor_);
  LOG_SENSOR("  ", "Output DC", this->output_dc_sensor_);
  LOG_SENSOR("  ", "Number of Cells", this->number_cells_sensor_);
  LOG_SENSOR("  ", "Battery Voltage", this->battery_voltage_sensor_);
  LOG_SENSOR("  ", "Battery Capacity", this->battery_capacity_sensor_);
  LOG_SENSOR("  ", "Battery Charge Current", this->battery_charge_current_sensor_);
  LOG_SENSOR("  ", "Battery Discharge Current", this->battery_discharge_current_sensor_);
  LOG_SENSOR("  ", "Inverter Bus Voltage", this->inv_bus_voltage_sensor_);
}

void UInverterSensor::handle_message(UInverterCmd cmd, std::string &data) {
  if (cmd == CMD_HSTS) {
    if (this->fault_code_sensor_ != nullptr)
      this->fault_code_sensor_->publish_state(parse_number<int>(data.substr(0, 2)).value());
  } else if (cmd == CMD_HGRID) {
    if (this->mains_voltage_sensor_ != nullptr)
      this->mains_voltage_sensor_->publish_state(parse_number<float>(data.substr(0, 5)).value());
    if (this->mains_frequency_sensor_ != nullptr)
      this->mains_frequency_sensor_->publish_state(parse_number<float>(data.substr(6, 4)).value());
    if (this->mains_lossv_high_sensor_ != nullptr)
      this->mains_lossv_high_sensor_->publish_state(parse_number<float>(data.substr(11, 3)).value());
    if (this->mains_lossv_low_sensor_ != nullptr)
      this->mains_lossv_low_sensor_->publish_state(parse_number<float>(data.substr(15, 3)).value());
    if (this->mains_lossf_high_sensor_ != nullptr)
      this->mains_lossf_high_sensor_->publish_state(parse_number<float>(data.substr(19, 3)).value());
    if (this->mains_lossf_low_sensor_ != nullptr)
      this->mains_lossf_low_sensor_->publish_state(parse_number<float>(data.substr(22, 3)).value());
    if (this->mains_power_sensor_ != nullptr)
      this->mains_power_sensor_->publish_state(parse_number<float>(data.substr(25, 5)).value());
  } else if (cmd == CMD_HOP) {
    if (this->output_voltage_sensor_ != nullptr)
      this->output_voltage_sensor_->publish_state(parse_number<float>(data.substr(0, 5)).value());
    if (this->output_frequency_sensor_ != nullptr)
      this->output_frequency_sensor_->publish_state(parse_number<float>(data.substr(6, 4)).value());
    if (this->output_reactive_power_sensor_ != nullptr)
      this->output_reactive_power_sensor_->publish_state(parse_number<float>(data.substr(11, 5)).value());
    if (this->output_active_power_sensor_ != nullptr)
      this->output_active_power_sensor_->publish_state(parse_number<float>(data.substr(17, 5)).value());
    if (this->load_percent_sensor_ != nullptr)
      this->load_percent_sensor_->publish_state(parse_number<float>(data.substr(23, 3)).value());
    if (this->output_dc_sensor_ != nullptr)
      this->output_dc_sensor_->publish_state(parse_number<float>(data.substr(27, 3)).value());
  } else if (cmd == CMD_HBAT) {
    if (this->number_cells_sensor_ != nullptr)
      this->number_cells_sensor_->publish_state(parse_number<float>(data.substr(0, 2)).value());
    if (this->battery_voltage_sensor_ != nullptr)
      this->battery_voltage_sensor_->publish_state(parse_number<float>(data.substr(3, 5)).value());
    if (this->battery_capacity_sensor_ != nullptr)
      this->battery_capacity_sensor_->publish_state(parse_number<float>(data.substr(9, 3)).value());
    if (this->battery_charge_current_sensor_ != nullptr)
      this->battery_charge_current_sensor_->publish_state(parse_number<float>(data.substr(13, 3)).value());
    if (this->battery_discharge_current_sensor_ != nullptr)
      this->battery_discharge_current_sensor_->publish_state(parse_number<float>(data.substr(17, 5)).value());
    if (this->inv_bus_voltage_sensor_ != nullptr)
      this->inv_bus_voltage_sensor_->publish_state(parse_number<float>(data.substr(23, 3)).value());
  } else if (cmd == CMD_HPV) {
    if (this->pv_voltage_sensor_ != nullptr)
      this->pv_voltage_sensor_->publish_state(parse_number<float>(data.substr(0, 5)).value());
    if (this->pv_current_sensor_ != nullptr)
      this->pv_current_sensor_->publish_state(parse_number<float>(data.substr(6, 4)).value());
    if (this->pv_power_sensor_ != nullptr)
      this->pv_power_sensor_->publish_state(parse_number<float>(data.substr(11, 5)).value());
  } else if (cmd == CMD_HPVB) {
    if (this->pvb_voltage_sensor_ != nullptr)
      this->pvb_voltage_sensor_->publish_state(parse_number<float>(data.substr(0, 5)).value());
    if (this->pvb_current_sensor_ != nullptr)
      this->pvb_current_sensor_->publish_state(parse_number<float>(data.substr(6, 4)).value());
    if (this->pvb_power_sensor_ != nullptr)
      this->pvb_power_sensor_->publish_state(parse_number<float>(data.substr(11, 5)).value());
  } else if (cmd == CMD_HTEMP) {
    if (this->pv_input_temp_sensor_ != nullptr)
      this->pv_input_temp_sensor_->publish_state(parse_number<float>(data.substr(0, 3)).value());
    if (this->inverter_temp_sensor_ != nullptr)
      this->inverter_temp_sensor_->publish_state(parse_number<float>(data.substr(4, 3)).value());
    if (this->boost_temp_sensor_ != nullptr)
      this->boost_temp_sensor_->publish_state(parse_number<float>(data.substr(8, 3)).value());
    if (this->transformer_temp_sensor_ != nullptr)
      this->transformer_temp_sensor_->publish_state(parse_number<float>(data.substr(12, 3)).value());
    if (this->hotspot_temp_sensor_ != nullptr)
      this->hotspot_temp_sensor_->publish_state(parse_number<float>(data.substr(16, 3)).value());
    if (this->fan1_speed_sensor_ != nullptr)
      this->fan1_speed_sensor_->publish_state(parse_number<float>(data.substr(20, 3)).value());
    if (this->fan2_speed_sensor_ != nullptr)
      this->fan2_speed_sensor_->publish_state(parse_number<float>(data.substr(24, 3)).value());
  }

}

}  // namespace u_inverter
}  // namespace esphome
