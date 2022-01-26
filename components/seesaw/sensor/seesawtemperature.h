#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "../seesaw.h"

namespace esphome {
namespace seesaw {

class SeesawTemperature : public sensor::Sensor, public PollingComponent {
 public:
  void setup() override;
  void update() override;

  void set_parent(Seesaw *parent) { parent_ = parent; }

 protected:
  Seesaw *parent_;
};

}  // namespace seesaw
}  // namespace esphome
