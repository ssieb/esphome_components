#include "esphome/core/hal.h"
#include "seesaw_binary_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace seesaw {

static const char *const TAG = "seesaw.binary_sensor";

void SeesawBinarySensor::setup() {
  this->parent_->set_pinmode(this->pin_, gpio::FLAG_PULLUP);
  this->parent_->set_gpio_interrupt(this->pin_, true);
  this->publish_initial_state(this->parent_->digital_read(this->pin_));
}

void SeesawBinarySensor::dump_config() {
  LOG_BINARY_SENSOR("", "Seesaw Binary Sensor", this);
  ESP_LOGCONFIG(TAG, "  Pin: %d", this->pin_);
}

void SeesawBinarySensor::loop() { this->publish_state(!this->parent_->digital_read(this->pin_)); }

}  // namespace seesaw
}  // namespace esphome
