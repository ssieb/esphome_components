#pragma once

#include "../u_inverter.h"
#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome {
namespace u_inverter {
class UInverterTextSensor : public UInverterListener, public Component {
 public:
  void dump_config() override;

  void set_operating_mode_tsensor(text_sensor::TextSensor *tsensor) { this->operating_mode_tsensor_ = tsensor; }

 protected:
  text_sensor::TextSensor *operating_mode_tsensor_{nullptr};

  void handle_message(UInverterCmd cmd, std::string &data) override;
};

}  // namespace u_inverter
}  // namespace esphome
