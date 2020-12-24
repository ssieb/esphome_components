#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/modbus/modbus.h"

namespace esphome {
namespace growatt {

class Growatt : public PollingComponent, public modbus::ModbusDevice {
 public:
  void set_input_power_sensor(sensor::Sensor *input_power_sensor) { input_power_sensor_ = input_power_sensor; }
  void set_pv1_voltage_sensor(sensor::Sensor *pv1_voltage_sensor) { pv1_voltage_sensor_ = pv1_voltage_sensor; }
  void set_pv1_current_sensor(sensor::Sensor *pv1_current_sensor) { pv1_current_sensor_ = pv1_current_sensor; }
  void set_pv1_power_sensor(sensor::Sensor *pv1_power_sensor) { pv1_power_sensor_ = pv1_power_sensor; }
  void set_pv2_voltage_sensor(sensor::Sensor *pv2_voltage_sensor) { pv2_voltage_sensor_ = pv2_voltage_sensor; }
  void set_pv2_current_sensor(sensor::Sensor *pv2_current_sensor) { pv2_current_sensor_ = pv2_current_sensor; }
  void set_pv2_power_sensor(sensor::Sensor *pv2_power_sensor) { pv2_power_sensor_ = pv2_power_sensor; }
  void set_output_power_sensor(sensor::Sensor *output_power_sensor) { output_power_sensor_ = output_power_sensor; }
  void set_grid_frequency_sensor(sensor::Sensor *grid_frequency_sensor) { grid_frequency_sensor_ = grid_frequency_sensor; }
  void set_ac_voltage_sensor(sensor::Sensor *ac_voltage_sensor) { ac_voltage_sensor_ = ac_voltage_sensor; }
  void set_ac_current_sensor(sensor::Sensor *ac_current_sensor) { ac_current_sensor_ = ac_current_sensor; }
  void set_ac_power_sensor(sensor::Sensor *ac_power_sensor) { ac_power_sensor_ = ac_power_sensor; }
  void set_today_gen_sensor(sensor::Sensor *today_gen_sensor) { today_gen_sensor_ = today_gen_sensor; }
  void set_total_gen_sensor(sensor::Sensor *total_gen_sensor) { total_gen_sensor_ = total_gen_sensor; }
  void set_temperature_sensor(sensor::Sensor *temperature_sensor) { temperature_sensor_ = temperature_sensor; }
  void set_today_grid_sensor(sensor::Sensor *today_grid_sensor) { today_grid_sensor_ = today_grid_sensor; }
  void set_total_grid_sensor(sensor::Sensor *total_grid_sensor) { total_grid_sensor_ = total_grid_sensor; }

  void loop() override;
  void update() override;

  void on_modbus_data(const std::vector<uint8_t> &data) override;

  void dump_config() override;

 protected:
  int state_{0};
  bool waiting_{false};
  long last_send_{0};

  sensor::Sensor *input_power_sensor_;
  sensor::Sensor *pv1_voltage_sensor_;
  sensor::Sensor *pv1_current_sensor_;
  sensor::Sensor *pv1_power_sensor_;
  sensor::Sensor *pv2_voltage_sensor_;
  sensor::Sensor *pv2_current_sensor_;
  sensor::Sensor *pv2_power_sensor_;
  sensor::Sensor *output_power_sensor_;
  sensor::Sensor *grid_frequency_sensor_;
  sensor::Sensor *ac_voltage_sensor_;
  sensor::Sensor *ac_current_sensor_;
  sensor::Sensor *ac_power_sensor_;
  sensor::Sensor *today_gen_sensor_;
  sensor::Sensor *total_gen_sensor_;
  sensor::Sensor *temperature_sensor_;
  sensor::Sensor *today_grid_sensor_;
  sensor::Sensor *total_grid_sensor_;
};

}  // namespace growatt
}  // namespace esphome
