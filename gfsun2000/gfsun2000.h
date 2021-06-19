#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/modbus/modbus.h"

namespace esphome {
namespace gfsun2000 {

class GFSun2000 : public PollingComponent, public modbus::ModbusDevice {
 public:
  void loop() override;
  void update() override;

  void on_modbus_data(const std::vector<uint8_t> &data) override;

  void dump_config() override;

  void set_device_id_sensor(text_sensor::TextSensor *device_id_sensor) { device_id_sensor_ = device_id_sensor; }
  void set_ac_voltage_sensor(sensor::Sensor *ac_voltage_sensor) { ac_voltage_sensor_ = ac_voltage_sensor; }
  void set_dc_voltage_sensor(sensor::Sensor *dc_voltage_sensor) { dc_voltage_sensor_ = dc_voltage_sensor; }
  void set_output_power_sensor(sensor::Sensor *output_power_sensor) { output_power_sensor_ = output_power_sensor; }
  void set_custom_energy_sensor(sensor::Sensor *custom_energy_sensor) { custom_energy_sensor_ = custom_energy_sensor; }
  void set_total_energy_sensor(sensor::Sensor *total_energy_sensor) { total_energy_sensor_ = total_energy_sensor; }

 protected:
  int state_{0};
  bool waiting_{false};
  long last_send_{0};

  text_sensor::TextSensor *device_id_sensor_;
  sensor::Sensor *ac_voltage_sensor_;
  sensor::Sensor *dc_voltage_sensor_;
  sensor::Sensor *output_power_sensor_;
  sensor::Sensor *custom_energy_sensor_;
  sensor::Sensor *total_energy_sensor_;
};

}  // namespace gfsun2000
}  // namespace esphome
