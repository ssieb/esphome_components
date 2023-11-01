#include "esphome/core/log.h"
#include "action_button.h"

namespace esphome {
namespace philips_series_3200 {

static const char *const TAG = "philips-action-button";

void ActionButton::dump_config() {
  LOG_BUTTON("", "Philips Action Button", this);
}

void ActionButton::press_action() {
  this->parent_->do_action(this->action_);
}

} // namespace philips_series_3200
} // namespace esphome
