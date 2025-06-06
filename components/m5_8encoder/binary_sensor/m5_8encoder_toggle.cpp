#include "m5_8encoder_toggle.h"
#include "esphome/core/log.h"

namespace esphome {
namespace m5_8encoder {

static const char *const TAG = "m5_8encoder.toggle";

void M5_8encoderToggle::dump_config() {
  LOG_BINARY_SENSOR("", "M5_8encoder toggle switch", this);
}

void M5_8encoderToggle::on_update(bool state) {
  this->publish_state(state);
}

}  // namespace m5_8encoder
}  // namespace esphome
