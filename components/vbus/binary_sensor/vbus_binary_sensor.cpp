#include "vbus_binary_sensor.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace vbus {

void DeltaSol_C_bsensor::dump_config() {
  ESP_LOGCONFIG(this->TAG_, "Deltasol C:");
}

void DeltaSol_C_bsensor::handle_message_(std::vector<uint8_t> &message) {
  if (this->s1_error_bsensor_ != nullptr)
    this->s1_error_bsensor_->publish_state(message[10] & 1);
  if (this->s2_error_bsensor_ != nullptr)
    this->s2_error_bsensor_->publish_state(message[10] & 2);
  if (this->s3_error_bsensor_ != nullptr)
    this->s3_error_bsensor_->publish_state(message[10] & 4);
  if (this->s4_error_bsensor_ != nullptr)
    this->s4_error_bsensor_->publish_state(message[10] & 8);
}

void DeltaSol_CS2_bsensor::dump_config() {
  ESP_LOGCONFIG(this->TAG_, "Deltasol CS2:");
}

void DeltaSol_CS2_bsensor::handle_message_(std::vector<uint8_t> &message) {
  if (this->s1_error_bsensor_ != nullptr)
    this->s1_error_bsensor_->publish_state(message[18] & 1);
  if (this->s2_error_bsensor_ != nullptr)
    this->s2_error_bsensor_->publish_state(message[18] & 2);
  if (this->s3_error_bsensor_ != nullptr)
    this->s3_error_bsensor_->publish_state(message[18] & 4);
  if (this->s4_error_bsensor_ != nullptr)
    this->s4_error_bsensor_->publish_state(message[18] & 8);
}

void DeltaSol_BS_Plus_bsensor::dump_config() {
  ESP_LOGCONFIG(this->TAG_, "Deltasol BS Plus:");
}

void DeltaSol_BS_Plus_bsensor::handle_message_(std::vector<uint8_t> &message) {
  if (this->relay1_bsensor_ != nullptr)
    this->relay1_bsensor_->publish_state(message[10] & 1);
  if (this->relay2_bsensor_ != nullptr)
    this->relay2_bsensor_->publish_state(message[10] & 2);
  if (this->s1_error_bsensor_ != nullptr)
    this->s1_error_bsensor_->publish_state(message[11] & 1);
  if (this->s2_error_bsensor_ != nullptr)
    this->s2_error_bsensor_->publish_state(message[11] & 2);
  if (this->s3_error_bsensor_ != nullptr)
    this->s3_error_bsensor_->publish_state(message[11] & 4);
  if (this->s4_error_bsensor_ != nullptr)
    this->s4_error_bsensor_->publish_state(message[11] & 8);
  if (this->collector_max_bsensor_ != nullptr)
    this->collector_max_bsensor_->publish_state(message[15] & 1);
  if (this->collector_min_bsensor_ != nullptr)
    this->collector_min_bsensor_->publish_state(message[15] & 2);
  if (this->collector_frost_bsensor_ != nullptr)
    this->collector_frost_bsensor_->publish_state(message[15] & 4);
  if (this->tube_collector_bsensor_ != nullptr)
    this->tube_collector_bsensor_->publish_state(message[15] & 8);
  if (this->recooling_bsensor_ != nullptr)
    this->recooling_bsensor_->publish_state(message[15] & 0x10);
  if (this->hqm_bsensor_ != nullptr)
    this->hqm_bsensor_->publish_state(message[15] & 0x20);
}

void VBusCustom_bsensor::dump_config() {
  ESP_LOGCONFIG(this->TAG_, "VBus Custom:");
}

void VBusCustom_bsensor::handle_message_(std::vector<uint8_t> &message) {
  if (this->message_handler_.has_value())
    (*this->message_handler_)(message);
}

}  // namespace vbus
}  // namespace esphome
