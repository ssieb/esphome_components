#include "vbus_sensor.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace vbus {

static inline uint16_t get_16(std::vector<uint8_t> &message, int start) {
  return (message[start + 1] << 8) + message[start];
}

void DeltaSol_C_sensor::dump_config() {
  ESP_LOGCONFIG(this->TAG_, "Deltasol C:");
}

void DeltaSol_C_sensor::handle_message_(std::vector<uint8_t> &message) {
  if (this->temperature1_sensor_ != nullptr)
    this->temperature1_sensor_->publish_state(get_16(message, 0) * 0.1f);
  if (this->temperature2_sensor_ != nullptr)
    this->temperature2_sensor_->publish_state(get_16(message, 2) * 0.1f);
  if (this->temperature3_sensor_ != nullptr)
    this->temperature3_sensor_->publish_state(get_16(message, 4) * 0.1f);
  if (this->temperature4_sensor_ != nullptr)
    this->temperature4_sensor_->publish_state(get_16(message, 6) * 0.1f);
  if (this->pump_speed1_sensor_ != nullptr)
    this->pump_speed1_sensor_->publish_state(message[8]);
  if (this->pump_speed2_sensor_ != nullptr)
    this->pump_speed2_sensor_->publish_state(message[9]);
  if (this->operating_hours1_sensor_ != nullptr)
    this->operating_hours1_sensor_->publish_state(get_16(message, 12));
  if (this->operating_hours2_sensor_ != nullptr)
    this->operating_hours2_sensor_->publish_state(get_16(message, 14));
  if (this->heat_quantity_sensor_ != nullptr)
    this->heat_quantity_sensor_->publish_state(get_16(message, 16) + get_16(message, 18) * 1000 + get_16(message, 20) * 1000000);
}

void DeltaSol_CS2_sensor::dump_config() {
  ESP_LOGCONFIG(this->TAG_, "Deltasol CS2:");
}

void DeltaSol_CS2_sensor::handle_message_(std::vector<uint8_t> &message) {
  if (this->temperature1_sensor_ != nullptr)
    this->temperature1_sensor_->publish_state(get_16(message, 0) * 0.1f);
  if (this->temperature2_sensor_ != nullptr)
    this->temperature2_sensor_->publish_state(get_16(message, 2) * 0.1f);
  if (this->temperature3_sensor_ != nullptr)
    this->temperature3_sensor_->publish_state(get_16(message, 4) * 0.1f);
  if (this->temperature4_sensor_ != nullptr)
    this->temperature4_sensor_->publish_state(get_16(message, 6) * 0.1f);
  if (this->temperature5_sensor_ != nullptr)
    this->temperature5_sensor_->publish_state(get_16(message, 8) * 0.1f);
  if (this->pump_speed_sensor_ != nullptr)
    this->pump_speed_sensor_->publish_state(message[12]);
  if (this->operating_hours_sensor_ != nullptr)
    this->operating_hours_sensor_->publish_state(get_16(message, 14));
  if (this->heat_quantity_sensor_ != nullptr)
    this->heat_quantity_sensor_->publish_state((get_16(message, 26) << 16) + get_16(message, 24));
}

void DeltaSol_BS_Plus_sensor::dump_config() {
  ESP_LOGCONFIG(this->TAG_, "Deltasol BS Plus:");
}

void DeltaSol_BS_Plus_sensor::handle_message_(std::vector<uint8_t> &message) {
  if (this->temperature1_sensor_ != nullptr)
    this->temperature1_sensor_->publish_state(get_16(message, 0) * 0.1f);
  if (this->temperature2_sensor_ != nullptr)
    this->temperature2_sensor_->publish_state(get_16(message, 2) * 0.1f);
  if (this->temperature3_sensor_ != nullptr)
    this->temperature3_sensor_->publish_state(get_16(message, 4) * 0.1f);
  if (this->temperature4_sensor_ != nullptr)
    this->temperature4_sensor_->publish_state(get_16(message, 6) * 0.1f);
  if (this->pump_speed1_sensor_ != nullptr)
    this->pump_speed1_sensor_->publish_state(message[8]);
  if (this->pump_speed2_sensor_ != nullptr)
    this->pump_speed2_sensor_->publish_state(message[9]);
  if (this->operating_hours1_sensor_ != nullptr)
    this->operating_hours1_sensor_->publish_state(get_16(message, 16));
  if (this->operating_hours2_sensor_ != nullptr)
    this->operating_hours2_sensor_->publish_state(get_16(message, 18));
  if (this->heat_quantity_sensor_ != nullptr)
    this->heat_quantity_sensor_->publish_state(get_16(message, 20) + get_16(message, 22) * 1000 + get_16(message, 24) * 1000000);
}

void VBusCustom_sensor::dump_config() {
  ESP_LOGCONFIG(this->TAG_, "VBus Custom:");
}

void VBusCustom_sensor::handle_message_(std::vector<uint8_t> &message) {
  if (this->message_handler_.has_value())
    (*this->message_handler_)(message);
}

}  // namespace vbus
}  // namespace esphome
