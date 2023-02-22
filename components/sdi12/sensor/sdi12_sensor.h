#pragma once

#include "../sdi12.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace sdi12 {

struct index_sensor {
  int index;
  sensor::Sensor *sensor;
};

class SDI12Sensor : public SDI12Listener, public PollingComponent {
 public:
  void dump_config() override;
  void update() override;
  void set_parent(SDI12 *parent) { this->parent_ = parent; }
  void add_sensor(int index, sensor::Sensor *sensor) { this->sensors_.push_back({index, sensor}); }

 protected:
  SDI12 *parent_;
  std::vector<struct index_sensor> sensors_;
  void handle_values(std::vector<float> &values) override;
};

}  // namespace sdi12
}  // namespace esphome
