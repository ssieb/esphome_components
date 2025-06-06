#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "../m5_8encoder.h"

namespace esphome {
namespace m5_8encoder {

class M5_8encoderSensor : public sensor::Sensor, public Component, public EncoderListener {
 public:
  void setup() override;
  void dump_config() override;

  void on_update(int32_t value) override;

  void set_number(uint8_t number) { this->index_ = number; }
  void set_min_value(int32_t min_value) { this->min_value_ = min_value; }
  void set_max_value(int32_t max_value) { this->max_value_ = max_value; }

 protected:
  uint8_t number_{0};
  int32_t value_{0};
  int32_t min_value_{INT32_MIN};
  int32_t max_value_{INT32_MAX};
};

}  // namespace m5_8encoder
}  // namespace esphome
