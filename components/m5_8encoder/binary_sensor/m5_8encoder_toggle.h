#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "../m5_8encoder.h"

namespace esphome {
namespace m5_8encoder {

class M5_8encoderToggle : public binary_sensor::BinarySensor, public Component, public ToggleListener {
 public:
  void dump_config() override;

  void on_update(bool state) override;
};

}  // namespace m5_8encoder
}  // namespace esphome
