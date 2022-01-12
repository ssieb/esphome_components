#pragma once

#include "esphome/core/component.h"

namespace esphome {
namespace crash {

class Crash : public Component {
 public:
  void loop() override;
};

}  // namespace crash
}  // namespace esphome


