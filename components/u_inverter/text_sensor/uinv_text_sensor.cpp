#include "uinv_text_sensor.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace u_inverter {

static const char *const TAG = "u_inverter.text_sensor";

void UInverterTextSensor::dump_config() {
  ESP_LOGCONFIG(TAG, "Unknown Inverter:");
  LOG_TEXT_SENSOR("  ", "Operating Mode", this->operating_mode_tsensor_);
}

void UInverterTextSensor::handle_message(UInverterCmd cmd, std::string &data) {
  if (cmd == CMD_HSTS) {
    if (this->operating_mode_tsensor_ != nullptr)
      this->operating_mode_tsensor_->publish_state(data.substr(3, 1));
  }
}

}  // namespace u_inverter
}  // namespace esphome
