#include "tri-state.h"
#include "esphome/core/log.h"

namespace esphome {
namespace tri_state {

static const char *const TAG = "switch.tri_state";

float TriStateSwitch::get_setup_priority() const { return setup_priority::HARDWARE; }

void TriStateSwitch::setup() {
  ESP_LOGCONFIG(TAG, "Setting up tri-state Switch '%s'...", this->name_.c_str());
  this->pin_->pin_mode(gpio::FLAG_INPUT);
}

void TriStateSwitch::dump_config() {
  LOG_SWITCH("", "GPIO Switch", this);
  LOG_PIN("  Pin: ", this->pin_);
  ESP_LOGCONFIG(TAG, "  On state: %s", ONOFF(this->on_state_));
}

void TriStateSwitch::write_state(bool state) {
  if (state) {
    this->pin_->pin_mode(gpio::FLAG_OUTPUT);
    this->pin_->digital_write(this->on_state_);
  } else {
    this->pin_->pin_mode(gpio::FLAG_INPUT);
  }
  this->publish_state(state);
}

}  // namespace tri_state
}  // namespace esphome
