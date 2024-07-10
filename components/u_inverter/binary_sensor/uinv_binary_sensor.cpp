#include "uinv_binary_sensor.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace u_inverter {

static const char *const TAG = "u_inverter.binary_sensor";

void UInverterBSensor::dump_config() {
  ESP_LOGCONFIG(TAG, "Unknown Inverter:");
  LOG_BINARY_SENSOR("  ", "Energy to Load", this->load_energy_bsensor_);
  LOG_BINARY_SENSOR("  ", "Has Output", this->has_output_bsensor_);
  LOG_BINARY_SENSOR("  ", "Low Battery", this->low_battery_bsensor_);
  LOG_BINARY_SENSOR("  ", "Battery Connected", this->battery_connected_bsensor_);
  LOG_BINARY_SENSOR("  ", "Output Overload", this->output_overload_bsensor_);
  LOG_BINARY_SENSOR("  ", "Over Temperature", this->over_temperature_bsensor_);
  LOG_BINARY_SENSOR("  ", "PV Power Low", this->pv_power_low_bsensor_);
  LOG_BINARY_SENSOR("  ", "Input Too High", this->input_too_high_bsensor_);
  LOG_BINARY_SENSOR("  ", "Battery Too High", this->battery_too_high_bsensor_);
  LOG_BINARY_SENSOR("  ", "Fan Speed Error", this->fan_speed_error_bsensor_);
  LOG_BINARY_SENSOR("  ", "Allow Battery Charging", this->allow_battery_charging_bsensor_);
  LOG_BINARY_SENSOR("  ", "Solar Battery Charging", this->solar_battery_charging_bsensor_);
  LOG_BINARY_SENSOR("  ", "Grid Battery Charging", this->grid_battery_charging_bsensor_);
}

void UInverterBSensor::handle_message(UInverterCmd cmd, std::string &data) {
  if (cmd == CMD_HSTS) {
    if (this->load_energy_bsensor_ != nullptr)
      this->load_energy_bsensor_->publish_state(data[4] != '0');
    if (this->has_output_bsensor_ != nullptr)
      this->has_output_bsensor_->publish_state(data[5] != '0');
    if (this->low_battery_bsensor_ != nullptr)
      this->low_battery_bsensor_->publish_state(data[6] != '0');
    if (this->battery_connected_bsensor_ != nullptr)
      this->battery_connected_bsensor_->publish_state(data[7] == '0');
    if (this->output_overload_bsensor_ != nullptr)
      this->output_overload_bsensor_->publish_state(data[8] != '0');
    if (this->over_temperature_bsensor_ != nullptr)
      this->over_temperature_bsensor_->publish_state(data[9] != '0');
    if (this->pv_power_low_bsensor_ != nullptr)
      this->pv_power_low_bsensor_->publish_state(data[12] != '0');
    if (this->input_too_high_bsensor_ != nullptr)
      this->input_too_high_bsensor_->publish_state(data[13] != '0');
    if (this->battery_too_high_bsensor_ != nullptr)
      this->battery_too_high_bsensor_->publish_state(data[14] != '0');
    if (this->fan_speed_error_bsensor_ != nullptr)
      this->fan_speed_error_bsensor_->publish_state(data[15] != '0');
  } else if (cmd == CMD_HBAT) {
    if (this->allow_battery_charging_bsensor_ != nullptr)
      this->allow_battery_charging_bsensor_->publish_state(data[27] != '0');
    if (this->solar_battery_charging_bsensor_ != nullptr)
      this->solar_battery_charging_bsensor_->publish_state(data[28] != '0');
    if (this->grid_battery_charging_bsensor_ != nullptr)
      this->grid_battery_charging_bsensor_->publish_state(data[29] != '0');
  } else if (cmd == CMD_HTEMP) {
    if (this->fan1_on_bsensor_ != nullptr)
      this->fan1_on_bsensor_->publish_state(data[28] != '0');
    if (this->fan2_on_bsensor_ != nullptr)
      this->fan2_on_bsensor_->publish_state(data[29] != '0');
  }
}

}  // namespace u_inverter
}  // namespace esphome
