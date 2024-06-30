#pragma once

#include "../u_inverter.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace u_inverter {
class UInverterSensor : public UInverterListener, public Component {
 public:
  void dump_config() override;

  void set_fault_code_sensor(sensor::Sensor *sensor) { this->fault_code_sensor_ = sensor; }

  void set_mains_voltage_sensor(sensor::Sensor *sensor) { this->mains_voltage_sensor_ = sensor; }
  void set_mains_frequency_sensor(sensor::Sensor *sensor) { this->mains_frequency_sensor_ = sensor; }
  void set_mains_lossv_high_sensor(sensor::Sensor *sensor) { this->mains_lossv_high_sensor_ = sensor; }
  void set_mains_lossv_low_sensor(sensor::Sensor *sensor) { this->mains_lossv_low_sensor_ = sensor; }
  void set_mains_lossf_high_sensor(sensor::Sensor *sensor) { this->mains_lossf_high_sensor_ = sensor; }
  void set_mains_lossf_low_sensor(sensor::Sensor *sensor) { this->mains_lossf_low_sensor_ = sensor; }
  void set_mains_power_sensor(sensor::Sensor *sensor) { this->mains_power_sensor_ = sensor; }

  void set_output_voltage_sensor(sensor::Sensor *sensor) { this->output_voltage_sensor_ = sensor; }
  void set_output_frequency_sensor(sensor::Sensor *sensor) { this->output_frequency_sensor_ = sensor; }
  void set_output_reactive_power_sensor(sensor::Sensor *sensor) { this->output_reactive_power_sensor_ = sensor; }
  void set_output_active_power_sensor(sensor::Sensor *sensor) { this->output_active_power_sensor_ = sensor; }
  void set_load_percent_sensor(sensor::Sensor *sensor) { this->load_percent_sensor_ = sensor; }
  void set_output_dc_sensor(sensor::Sensor *sensor) { this->output_dc_sensor_ = sensor; }

  void set_number_cells_sensor(sensor::Sensor *sensor) { this->number_cells_sensor_ = sensor; }
  void set_battery_voltage_sensor(sensor::Sensor *sensor) { this->battery_voltage_sensor_ = sensor; }
  void set_battery_capacity_sensor(sensor::Sensor *sensor) { this->battery_capacity_sensor_ = sensor; }
  void set_battery_charge_current_sensor(sensor::Sensor *sensor) { this->battery_charge_current_sensor_ = sensor; }
  void set_battery_discharge_current_sensor(sensor::Sensor *sensor) { this->battery_discharge_current_sensor_ = sensor; }
  void set_inv_bus_voltage_sensor(sensor::Sensor *sensor) { this->inv_bus_voltage_sensor_ = sensor; }

 protected:
  sensor::Sensor *fault_code_sensor_{nullptr};

  sensor::Sensor *mains_voltage_sensor_{nullptr};
  sensor::Sensor *mains_frequency_sensor_{nullptr};
  sensor::Sensor *mains_lossv_high_sensor_{nullptr};
  sensor::Sensor *mains_lossv_low_sensor_{nullptr};
  sensor::Sensor *mains_lossf_high_sensor_{nullptr};
  sensor::Sensor *mains_lossf_low_sensor_{nullptr};
  sensor::Sensor *mains_power_sensor_{nullptr};

  sensor::Sensor *output_voltage_sensor_{nullptr};
  sensor::Sensor *output_frequency_sensor_{nullptr};
  sensor::Sensor *output_reactive_power_sensor_{nullptr};
  sensor::Sensor *output_active_power_sensor_{nullptr};
  sensor::Sensor *load_percent_sensor_{nullptr};
  sensor::Sensor *output_dc_sensor_{nullptr};

  sensor::Sensor *number_cells_sensor_{nullptr};
  sensor::Sensor *battery_voltage_sensor_{nullptr};
  sensor::Sensor *battery_capacity_sensor_{nullptr};
  sensor::Sensor *battery_charge_current_sensor_{nullptr};
  sensor::Sensor *battery_discharge_current_sensor_{nullptr};
  sensor::Sensor *inv_bus_voltage_sensor_{nullptr};

  void handle_message(UInverterCmd cmd, std::string &data) override;
};

}  // namespace u_inverter
}  // namespace esphome
