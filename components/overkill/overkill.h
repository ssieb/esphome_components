#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace overkill {

struct numbered_sensor {
  int num;
  sensor::Sensor *sensor;
};

class Overkill : public PollingComponent, public uart::UARTDevice {
 public:
  void add_battery_voltage_sensor(int snum, sensor::Sensor *sensor) { battery_voltage_sensors_.push_back({snum - 1, sensor}); }
  void add_temperature_sensor(int snum, sensor::Sensor *sensor) { temperature_sensors_.push_back({snum, sensor}); }
  void set_voltage_sensor(sensor::Sensor *sensor) { voltage_sensor_ = sensor; }
  void set_current_sensor(sensor::Sensor *sensor) { current_sensor_ = sensor; }
  void set_balance_capacity_sensor(sensor::Sensor *sensor) { balance_capacity_sensor_ = sensor; }
  void set_rate_capacity_sensor(sensor::Sensor *sensor) { rate_capacity_sensor_ = sensor; }
  void set_capacity_sensor(sensor::Sensor *sensor) { capacity_sensor_ = sensor; }

  void dump_config() override;
  void loop() override;
  void update() override;

  float get_setup_priority() const { return setup_priority::DATA; }

 protected:
  bool read_data_(bool first=false);
  void parse_data_(uint8_t cmd);
  uint16_t get_16_bit_uint_(uint8_t start_index);

  std::vector<struct numbered_sensor> battery_voltage_sensors_;
  std::vector<struct numbered_sensor> temperature_sensors_;
  sensor::Sensor *voltage_sensor_{nullptr};
  sensor::Sensor *current_sensor_{nullptr};
  sensor::Sensor *balance_capacity_sensor_{nullptr};
  sensor::Sensor *rate_capacity_sensor_{nullptr};
  sensor::Sensor *capacity_sensor_{nullptr};

  std::vector<uint8_t> data_;
  int string_count_{0};
  int ntc_count_{0};
};

}  // namespace overkill
}  // namespace esphome
