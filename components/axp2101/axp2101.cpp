#include "axp2101.h"
#include "esphome/core/log.h"

namespace esphome {
namespace axp2101 {

static const char *const TAG = "axp2101.sensor";

static const uint8_t AXP2101_REGISTER_PMU_STATUS2 = 0x01;
static const uint8_t AXP2101_REGISTER_BATTERY_LEVEL = 0xA4;

float AXP2101::get_setup_priority() const { return setup_priority::DATA; }

void AXP2101::update() {
  uint8_t data;
  if (this->read_register(AXP2101_REGISTER_BATTERY_LEVEL, &data, 1) != i2c::NO_ERROR) {
    ESP_LOGE(TAG, "Unable to read from device");
    return;
  }
  if (this->battery_level_ != nullptr)
    this->battery_level_->publish_state(data);
}

void AXP2101::setup() {
  ESP_LOGCONFIG(TAG, "Setting up AXP2101...");
}

void AXP2101::loop() {
  uint8_t data;
  if (this->charging_ != nullptr) {
    if (this->read_register(AXP2101_REGISTER_PMU_STATUS2, &data, 1) != i2c::NO_ERROR)
      return;
    this->charging_->publish_state((data & 0x60) == 0x20);
  }
}

void AXP2101::dump_config() {
  ESP_LOGCONFIG(TAG, "AXP2101:");
  LOG_I2C_DEVICE(this);
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Connection with AXP2101 failed!");
  }
  LOG_UPDATE_INTERVAL(this);
  LOG_SENSOR("  ", "Battery Level", this->battery_level_);
}

}  // namespace axp2101
}  // namespace esphome
