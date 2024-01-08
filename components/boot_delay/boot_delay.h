#pragma once

#include "esphome/core/component.h"

namespace esphome {
namespace boot_delay {

class BootDelay : public Component {
 public:
  bool can_proceed() override;
  void set_delay(uint32_t delay) { this->delay_ = delay; }

 protected:
  uint32_t delay_;
};

}  // namespace boot_delay
}  // namespace esphome

