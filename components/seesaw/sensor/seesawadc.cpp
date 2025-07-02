#include "seesawadc.h"
#include "esphome/core/log.h"

namespace esphome {
namespace seesaw {

static const char *const TAG = "seesaw.adc";

void SeesawADC::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Seesaw touch sensor...");
}

void SeesawADC::update() {
  uint16_t value = this->parent_->analog_read(this->pin_);
  this->publish_state(value);
}

}  // namespace seesaw
}  // namespace esphome
