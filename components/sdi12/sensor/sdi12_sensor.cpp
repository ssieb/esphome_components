#include "sdi12_sensor.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace sdi12 {

static const char *const TAG = "sdi12.sensor";

void SDI12Sensor::dump_config() {
  ESP_LOGCONFIG(TAG, "SDI12 Sensor:");
  ESP_LOGCONFIG(TAG, "  Address: %d", this->address_);
  ESP_LOGCONFIG(TAG, "  Sensors:");
  for (auto is : this->sensors_)
    LOG_SENSOR("  ", "-", is.sensor);
}

void SDI12Sensor::loop() {
  if (this->want_update_ && this->parent_->start_measurement(this->address_)) {
    this->want_update_ = false;
    this->disable_loop();
  }
}

void SDI12Sensor::update() {
  if (this->want_update_) {
    ESP_LOGW(TAG, "still waiting for previous update");
  } else {
    this->want_update_ = true;
    this->enable_loop();
  }
}

void SDI12Sensor::handle_values(std::vector<float> &values) {
  for (auto is : this->sensors_)
    if (is.index < values.size())
      is.sensor->publish_state(values[is.index]);
}

}  // namespace sdi12
}  // namespace esphome
