#include "uinv_text_sensor.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace u_inverter {

static const char *const TAG = "u_inverter.text_sensor";

void UInverterTextSensor::dump_config() {
  ESP_LOGCONFIG(TAG, "Unknown Inverter Text Sensor:");
  LOG_TEXT_SENSOR("  ", "Operating Mode", this->operating_mode_tsensor_);
  LOG_TEXT_SENSOR("  ", "Working Mode", this->working_mode_tsensor_);
  LOG_TEXT_SENSOR("  ", "Grid Voltage Range", this->grid_voltage_range_tsensor_);
  LOG_TEXT_SENSOR("  ", "Battery Type", this->battery_type_tsensor_);
  LOG_TEXT_SENSOR("  ", "System Time", this->system_time_tsensor_);
}

void UInverterTextSensor::handle_message(UInverterCmd cmd, std::string &data) {
  if (cmd == CMD_HSTS) {
    if (this->operating_mode_tsensor_ != nullptr)
      this->operating_mode_tsensor_->publish_state(data.substr(3, 1));
  } else if (cmd == CMD_HEEP1) {
    if (this->working_mode_tsensor_ != nullptr)
      this->working_mode_tsensor_->publish_state(data.substr(0, 1));
    if (this->grid_voltage_range_tsensor_ != nullptr)
      this->grid_voltage_range_tsensor_->publish_state(data.substr(10, 1));
    if (this->battery_type_tsensor_ != nullptr)
      this->battery_type_tsensor_->publish_state(data.substr(12, 1));
  } else if (cmd == CMD_HGEN) {
    if (this->system_time_tsensor_ != nullptr) {
      std::string systime = "20" + data.substr(0, 2) + "-" + data.substr(2, 2) + "-" + data.substr(4, 2) + " " + data.substr(7, 5);
      this->system_time_tsensor_->publish_state(systime);
    }
  }
}

}  // namespace u_inverter
}  // namespace esphome
