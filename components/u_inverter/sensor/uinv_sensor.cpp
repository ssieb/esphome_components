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
}

void UInverterSensor::handle_message(UInverterCmd cmd, std::string &data) {
  if (cmd == CMD_HSTS) {
    if (this->fault_code_sensor_ != nullptr)
      this->fault_code_sensor_->publish_state(parse_number<int>(data.substr(0, 2)).value());
  } else if (cmd == CMD_HGRID) {
    if (this->mains_voltage_sensor_ != nullptr)
      this->mains_voltage_sensor_->publish_state(parse_number<int>(data.substr(0, 5)).value());
    if (this->mains_frequency_sensor_ != nullptr)
      this->mains_frequency_sensor_->publish_state(parse_number<int>(data.substr(6, 4)).value());
    if (this->mains_lossv_high_sensor_ != nullptr)
      this->mains_lossv_high_sensor_->publish_state(parse_number<int>(data.substr(11, 3)).value());
    if (this->mains_lossv_low_sensor_ != nullptr)
      this->mains_lossv_low_sensor_->publish_state(parse_number<int>(data.substr(15, 3)).value());
    if (this->mains_lossf_high_sensor_ != nullptr)
      this->mains_lossf_high_sensor_->publish_state(parse_number<int>(data.substr(19, 3)).value());
    if (this->mains_lossf_low_sensor_ != nullptr)
      this->mains_lossf_low_sensor_->publish_state(parse_number<int>(data.substr(22, 3)).value());
    if (this->mains_power_sensor_ != nullptr)
      this->mains_power_sensor_->publish_state(parse_number<int>(data.substr(25, 5)).value());
  }
}

}  // namespace u_inverter
}  // namespace esphome
