#include "esphome/core/hal.h"
#include "seesaw_binary_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace seesaw {

static const char *const TAG = "seesaw.binary_sensor";

void SeesawBinarySensor::setup() {
  this->parent_->set_pinmode(24, gpio::FLAG_PULLUP);
  this->parent_->set_gpio_interrupt(24, true);
  this->publish_initial_state(this->parent_->digital_read(24));
}

void SeesawBinarySensor::dump_config() {
  LOG_BINARY_SENSOR("", "Seesaw Binary Sensor", this);
}

void SeesawBinarySensor::loop() { this->publish_state(this->parent_->digital_read(24)); }

}  // namespace seesaw
}  // namespace esphome
