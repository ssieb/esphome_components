#pragma once

#include "../u_inverter.h"
#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome {
namespace u_inverter {
class UInverterTextSensor : public UInverterListener, public Component {
 public:
  void dump_config() override;

  void set_operating_mode_tsensor(text_sensor::TextSensor *tsensor) { this->operating_mode_tsensor_ = tsensor; }

  void set_working_mode_tsensor(text_sensor::TextSensor *tsensor) { this->working_mode_tsensor_ = tsensor; }
  void set_grid_voltage_range_tsensor(text_sensor::TextSensor *tsensor) { this->grid_voltage_range_tsensor_ = tsensor; }
  void set_battery_type_tsensor(text_sensor::TextSensor *tsensor) { this->battery_type_tsensor_ = tsensor; }

  void set_system_time_tsensor(text_sensor::TextSensor *tsensor) { this->system_time_tsensor_ = tsensor; }

 protected:
  text_sensor::TextSensor *operating_mode_tsensor_{nullptr};

  text_sensor::TextSensor *working_mode_tsensor_{nullptr};
  text_sensor::TextSensor *grid_voltage_range_tsensor_{nullptr};
  text_sensor::TextSensor *battery_type_tsensor_{nullptr};

  text_sensor::TextSensor *system_time_tsensor_{nullptr};

  void handle_message(UInverterCmd cmd, std::string &data) override;
};

}  // namespace u_inverter
}  // namespace esphome
