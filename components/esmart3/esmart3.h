#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace esmart3 {

class ESmart3Component : public PollingComponent, public uart::UARTDevice {
 public:
  void set_charge_mode_sensor(sensor::Sensor *charge_mode_sensor) { charge_mode_sensor_ = charge_mode_sensor; }
  void set_input_voltage_sensor(sensor::Sensor *input_voltage_sensor) { input_voltage_sensor_ = input_voltage_sensor; }
  void set_battery_voltage_sensor(sensor::Sensor *battery_voltage_sensor) { battery_voltage_sensor_ = battery_voltage_sensor; }
  void set_charging_current_sensor(sensor::Sensor *charging_current_sensor) { charging_current_sensor_ = charging_current_sensor; }
  void set_load_voltage_sensor(sensor::Sensor *load_voltage_sensor) { load_voltage_sensor_ = load_voltage_sensor; }
  void set_load_current_sensor(sensor::Sensor *load_current_sensor) { load_current_sensor_ = load_current_sensor; }
  void set_charging_power_sensor(sensor::Sensor *charging_power_sensor) { charging_power_sensor_ = charging_power_sensor; }
  void set_load_power_sensor(sensor::Sensor *load_power_sensor) { load_power_sensor_ = load_power_sensor; }
  void set_battery_temp_sensor(sensor::Sensor *battery_temp_sensor) { battery_temp_sensor_ = battery_temp_sensor; }
  void set_internal_temp_sensor(sensor::Sensor *internal_temp_sensor) { internal_temp_sensor_ = internal_temp_sensor; }
  void set_battery_level_sensor(sensor::Sensor *battery_level_sensor) { battery_level_sensor_ = battery_level_sensor; }

  void dump_config() override;
  void loop() override;
  void update() override;

  float get_setup_priority() const { return setup_priority::DATA; }

 protected:
  bool check_data_() const;
  void parse_data_();
  uint16_t get_16_bit_uint_(uint8_t start_index) const;

  sensor::Sensor *charge_mode_sensor_{nullptr};
  sensor::Sensor *input_voltage_sensor_{nullptr};
  sensor::Sensor *battery_voltage_sensor_{nullptr};
  sensor::Sensor *charging_current_sensor_{nullptr};
  sensor::Sensor *load_voltage_sensor_{nullptr};
  sensor::Sensor *load_current_sensor_{nullptr};
  sensor::Sensor *charging_power_sensor_{nullptr};
  sensor::Sensor *load_power_sensor_{nullptr};
  sensor::Sensor *battery_temp_sensor_{nullptr};
  sensor::Sensor *internal_temp_sensor_{nullptr};
  sensor::Sensor *battery_level_sensor_{nullptr};

  std::vector<uint8_t> data_;
  bool receiving_{false};
  uint8_t data_count_;
  uint32_t last_transmission_{0};
};

}  // namespace esmart3
}  // namespace esphome
