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

  void set_pv_voltage_sensor(sensor::Sensor *sensor) { this->pv_voltage_sensor_ = sensor; }
  void set_pv_current_sensor(sensor::Sensor *sensor) { this->pv_current_sensor_ = sensor; }
  void set_pv_power_sensor(sensor::Sensor *sensor) { this->pv_power_sensor_ = sensor; }

  void set_pvb_voltage_sensor(sensor::Sensor *sensor) { this->pvb_voltage_sensor_ = sensor; }
  void set_pvb_current_sensor(sensor::Sensor *sensor) { this->pvb_current_sensor_ = sensor; }
  void set_pvb_power_sensor(sensor::Sensor *sensor) { this->pvb_power_sensor_ = sensor; }

  void set_pv_input_temp_sensor(sensor::Sensor *sensor) { this->pv_input_temp_sensor_ = sensor; }
  void set_inverter_temp_sensor(sensor::Sensor *sensor) { this->inverter_temp_sensor_ = sensor; }
  void set_boost_temp_sensor(sensor::Sensor *sensor) { this->boost_temp_sensor_ = sensor; }
  void set_transformer_temp_sensor(sensor::Sensor *sensor) { this->transformer_temp_sensor_ = sensor; }
  void set_hotspot_temp_sensor(sensor::Sensor *sensor) { this->hotspot_temp_sensor_ = sensor; }
  void set_fan1_speed_sensor(sensor::Sensor *sensor) { this->fan1_speed_sensor_ = sensor; }
  void set_fan2_speed_sensor(sensor::Sensor *sensor) { this->fan2_speed_sensor_ = sensor; }

  void set_max_total_charging_current_sensor(sensor::Sensor *sensor) { this->max_total_charging_current_sensor_ = sensor; }
  void set_max_grid_charging_current_sensor(sensor::Sensor *sensor) { this->max_grid_charging_current_sensor_ = sensor; }
  void set_output_set_voltage_sensor(sensor::Sensor *sensor) { this->output_set_voltage_sensor_ = sensor; }
  void set_target_charging_voltage_sensor(sensor::Sensor *sensor) { this->target_charging_voltage_sensor_ = sensor; }
  void set_battery_float_voltage_sensor(sensor::Sensor *sensor) { this->battery_float_voltage_sensor_ = sensor; }
  void set_battery_power_off_voltage_sensor(sensor::Sensor *sensor) { this->battery_power_off_voltage_sensor_ = sensor; }

  void set_daily_energy_sensor(sensor::Sensor *sensor) { this->daily_energy_sensor_ = sensor; }
  void set_monthly_energy_sensor(sensor::Sensor *sensor) { this->monthly_energy_sensor_ = sensor; }
  void set_annual_energy_sensor(sensor::Sensor *sensor) { this->annual_energy_sensor_ = sensor; }
  void set_total_energy_sensor(sensor::Sensor *sensor) { this->total_energy_sensor_ = sensor; }

 protected:
  sensor::Sensor *fault_code_sensor_{nullptr};

  sensor::Sensor *mains_voltage_sensor_{nullptr};
  sensor::Sensor *mains_frequency_sensor_{nullptr};
  sensor::Sensor *mains_lossv_high_sensor_{nullptr};
  sensor::Sensor *mains_lossv_low_sensor_{nullptr};
  sensor::Sensor *mains_lossf_high_sensor_{nullptr};
  sensor::Sensor *mains_lossf_low_sensor_{nullptr};

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

  sensor::Sensor *pv_voltage_sensor_{nullptr};
  sensor::Sensor *pv_current_sensor_{nullptr};
  sensor::Sensor *pv_power_sensor_{nullptr};

  sensor::Sensor *pvb_voltage_sensor_{nullptr};
  sensor::Sensor *pvb_current_sensor_{nullptr};
  sensor::Sensor *pvb_power_sensor_{nullptr};

  sensor::Sensor *pv_input_temp_sensor_{nullptr};
  sensor::Sensor *inverter_temp_sensor_{nullptr};
  sensor::Sensor *boost_temp_sensor_{nullptr};
  sensor::Sensor *transformer_temp_sensor_{nullptr};
  sensor::Sensor *hotspot_temp_sensor_{nullptr};
  sensor::Sensor *fan1_speed_sensor_{nullptr};
  sensor::Sensor *fan2_speed_sensor_{nullptr};

  sensor::Sensor *max_total_charging_current_sensor_{nullptr};
  sensor::Sensor *max_grid_charging_current_sensor_{nullptr};
  sensor::Sensor *output_set_voltage_sensor_{nullptr};
  sensor::Sensor *target_charging_voltage_sensor_{nullptr};
  sensor::Sensor *battery_float_voltage_sensor_{nullptr};
  sensor::Sensor *battery_power_off_voltage_sensor_{nullptr};

  sensor::Sensor *daily_energy_sensor_{nullptr};
  sensor::Sensor *monthly_energy_sensor_{nullptr};
  sensor::Sensor *annual_energy_sensor_{nullptr};
  sensor::Sensor *total_energy_sensor_{nullptr};

  void handle_message(UInverterCmd cmd, std::string &data) override;
};

}  // namespace u_inverter
}  // namespace esphome
