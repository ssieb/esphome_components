#include "boot_delay.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace boot_delay {

bool BootDelay::can_proceed() {
  return millis() >= this->delay_;
}

}  // namespace boot_delay
}  // namespace esphome

