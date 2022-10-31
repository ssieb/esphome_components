#include "vbus_text_sensor.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace vbus {

static inline uint16_t get_16(std::vector<uint8_t> &message, int start) {
  return (message[start + 1] << 8) + message[start];
}

void DeltaSol_C_tsensor::dump_config() {
  ESP_LOGCONFIG(this->TAG_, "Deltasol C:");
}

void DeltaSol_C_tsensor::handle_message_(std::vector<uint8_t> &message) {
  uint16_t value;
  if (this->time_tsensor_ != nullptr) {
    value = get_16(message, 22);
    this->time_tsensor_->publish_state(str_sprintf("%2d:%02d", value / 60, value % 60));
  }
}

void DeltaSol_CS2_tsensor::dump_config() {
  ESP_LOGCONFIG(this->TAG_, "Deltasol CS2:");
}

void DeltaSol_CS2_tsensor::handle_message_(std::vector<uint8_t> &message) {
  uint16_t value;
  if (this->version_tsensor_ != nullptr) {
    value = get_16(message, 28);
    this->version_tsensor_->publish_state(str_sprintf("%d.%02d", value / 100, value % 100));
  }
}

void DeltaSol_BS_Plus_tsensor::dump_config() {
  ESP_LOGCONFIG(this->TAG_, "Deltasol BS Plus:");
}

void DeltaSol_BS_Plus_tsensor::handle_message_(std::vector<uint8_t> &message) {
  uint16_t value;
  if (this->time_tsensor_ != nullptr) {
    value = get_16(message, 12);
    this->time_tsensor_->publish_state(str_sprintf("%2d:%02d", value / 60, value % 60));
  }
  if (this->version_tsensor_ != nullptr) {
    value = get_16(message, 26);
    this->version_tsensor_->publish_state(str_sprintf("%d.%02d", value / 100, value % 100));
  }
}

void VBusCustom_tsensor::dump_config() {
  ESP_LOGCONFIG(this->TAG_, "VBus Custom:");
}

void VBusCustom_tsensor::handle_message_(std::vector<uint8_t> &message) {
  if (this->message_handler_.has_value())
    (*this->message_handler_)(message);
}

}  // namespace vbus
}  // namespace esphome
