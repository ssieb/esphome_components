#pragma once

#include "../u_inverter.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome {
namespace u_inverter {

class UInverterBSensor : public UInverterListener, public Component {
 public:
  void dump_config() override;

  // HSTS
  void set_load_energy_bsensor(binary_sensor::BinarySensor *bsensor) { this->load_energy_bsensor_ = bsensor; }
  void set_has_output_bsensor(binary_sensor::BinarySensor *bsensor) { this->has_output_bsensor_ = bsensor; }
  void set_low_battery_bsensor(binary_sensor::BinarySensor *bsensor) { this->low_battery_bsensor_ = bsensor; }
  void set_battery_connected_bsensor(binary_sensor::BinarySensor *bsensor) { this->battery_connected_bsensor_ = bsensor; }
  void set_output_overload_bsensor(binary_sensor::BinarySensor *bsensor) { this->output_overload_bsensor_ = bsensor; }
  void set_over_temperature_bsensor(binary_sensor::BinarySensor *bsensor) { this->over_temperature_bsensor_ = bsensor; }
  void set_pv_power_low_bsensor(binary_sensor::BinarySensor *bsensor) { this->pv_power_low_bsensor_ = bsensor; }
  void set_input_too_high_bsensor(binary_sensor::BinarySensor *bsensor) { this->input_too_high_bsensor_ = bsensor; }
  void set_battery_too_high_bsensor(binary_sensor::BinarySensor *bsensor) { this->battery_too_high_bsensor_ = bsensor; }
  void set_fan_speed_error_bsensor(binary_sensor::BinarySensor *bsensor) { this->fan_speed_error_bsensor_ = bsensor; }

  // HBAT
  void set_allow_battery_charging_bsensor(binary_sensor::BinarySensor *bsensor) { this->allow_battery_charging_bsensor_ = bsensor; }
  void set_solar_battery_charging_bsensor(binary_sensor::BinarySensor *bsensor) { this->solar_battery_charging_bsensor_ = bsensor; }
  void set_grid_battery_charging_bsensor(binary_sensor::BinarySensor *bsensor) { this->grid_battery_charging_bsensor_ = bsensor; }

  // HTEMP
  void set_fan1_on_bsensor(binary_sensor::BinarySensor *bsensor) { this->fan1_on_bsensor_ = bsensor; }
  void set_fan2_on_bsensor(binary_sensor::BinarySensor *bsensor) { this->fan2_on_bsensor_ = bsensor; }

 protected:
  binary_sensor::BinarySensor *load_energy_bsensor_{nullptr};
  binary_sensor::BinarySensor *has_output_bsensor_{nullptr};
  binary_sensor::BinarySensor *low_battery_bsensor_{nullptr};
  binary_sensor::BinarySensor *battery_connected_bsensor_{nullptr};
  binary_sensor::BinarySensor *output_overload_bsensor_{nullptr};
  binary_sensor::BinarySensor *over_temperature_bsensor_{nullptr};
  binary_sensor::BinarySensor *pv_power_low_bsensor_{nullptr};
  binary_sensor::BinarySensor *input_too_high_bsensor_{nullptr};
  binary_sensor::BinarySensor *battery_too_high_bsensor_{nullptr};
  binary_sensor::BinarySensor *fan_speed_error_bsensor_{nullptr};

  binary_sensor::BinarySensor *allow_battery_charging_bsensor_{nullptr};
  binary_sensor::BinarySensor *solar_battery_charging_bsensor_{nullptr};
  binary_sensor::BinarySensor *grid_battery_charging_bsensor_{nullptr};

  binary_sensor::BinarySensor *fan1_on_bsensor_{nullptr};
  binary_sensor::BinarySensor *fan2_on_bsensor_{nullptr};

  void handle_message(UInverterCmd cmd, std::string &data) override;
};

}  // namespace u_inverter
}  // namespace esphome
