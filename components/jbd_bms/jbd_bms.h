#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace jbd_bms {

struct numbered_sensor {
  int num;
  sensor::Sensor *sensor;
};

struct numbered_bsensor {
  int num;
  binary_sensor::BinarySensor *sensor;
};

class JBD_BMS : public PollingComponent, public uart::UARTDevice {
 public:
  void add_battery_voltage_sensor(int snum, sensor::Sensor *sensor) { battery_voltage_sensors_.push_back({snum - 1, sensor}); }
  void add_temperature_sensor(int snum, sensor::Sensor *sensor) { temperature_sensors_.push_back({snum, sensor}); }
  void set_voltage_sensor(sensor::Sensor *sensor) { voltage_sensor_ = sensor; }
  void set_current_sensor(sensor::Sensor *sensor) { current_sensor_ = sensor; }
  void set_balance_capacity_sensor(sensor::Sensor *sensor) { balance_capacity_sensor_ = sensor; }
  void set_rate_capacity_sensor(sensor::Sensor *sensor) { rate_capacity_sensor_ = sensor; }
  void set_capacity_sensor(sensor::Sensor *sensor) { capacity_sensor_ = sensor; }
  void set_charge_sensor(binary_sensor::BinarySensor *sensor) { charge_sensor_ = sensor; }
  void set_discharge_sensor(binary_sensor::BinarySensor *sensor) { discharge_sensor_ = sensor; }
  void set_prot_covp_sensor(binary_sensor::BinarySensor *sensor) { prot_covp_sensor_ = sensor; }
  void set_prot_cuvp_sensor(binary_sensor::BinarySensor *sensor) { prot_cuvp_sensor_ = sensor; }
  void set_prot_povp_sensor(binary_sensor::BinarySensor *sensor) { prot_povp_sensor_ = sensor; }
  void set_prot_puvp_sensor(binary_sensor::BinarySensor *sensor) { prot_puvp_sensor_ = sensor; }
  void set_prot_chgot_sensor(binary_sensor::BinarySensor *sensor) { prot_chgot_sensor_ = sensor; }
  void set_prot_chgut_sensor(binary_sensor::BinarySensor *sensor) { prot_chgut_sensor_ = sensor; }
  void set_prot_dsgot_sensor(binary_sensor::BinarySensor *sensor) { prot_dsgot_sensor_ = sensor; }
  void set_prot_dsgut_sensor(binary_sensor::BinarySensor *sensor) { prot_dsgut_sensor_ = sensor; }
  void set_prot_chgoc_sensor(binary_sensor::BinarySensor *sensor) { prot_chgoc_sensor_ = sensor; }
  void set_prot_dsgoc_sensor(binary_sensor::BinarySensor *sensor) { prot_dsgoc_sensor_ = sensor; }
  void set_prot_short_sensor(binary_sensor::BinarySensor *sensor) { prot_short_sensor_ = sensor; }
  void set_prot_afe_sensor(binary_sensor::BinarySensor *sensor) { prot_afe_sensor_ = sensor; }
  void set_swlock_sensor(binary_sensor::BinarySensor *sensor) { swlock_sensor_ = sensor; }
  void add_balance_state_sensor(int snum, binary_sensor::BinarySensor *sensor) { balance_state_sensors_.push_back({snum - 1, sensor}); }

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
  binary_sensor::BinarySensor *charge_sensor_{nullptr};
  binary_sensor::BinarySensor *discharge_sensor_{nullptr};
  binary_sensor::BinarySensor *prot_covp_sensor_{nullptr};
  binary_sensor::BinarySensor *prot_cuvp_sensor_{nullptr};
  binary_sensor::BinarySensor *prot_povp_sensor_{nullptr};
  binary_sensor::BinarySensor *prot_puvp_sensor_{nullptr};
  binary_sensor::BinarySensor *prot_chgot_sensor_{nullptr};
  binary_sensor::BinarySensor *prot_chgut_sensor_{nullptr};
  binary_sensor::BinarySensor *prot_dsgot_sensor_{nullptr};
  binary_sensor::BinarySensor *prot_dsgut_sensor_{nullptr};
  binary_sensor::BinarySensor *prot_chgoc_sensor_{nullptr};
  binary_sensor::BinarySensor *prot_dsgoc_sensor_{nullptr};
  binary_sensor::BinarySensor *prot_short_sensor_{nullptr};
  binary_sensor::BinarySensor *prot_afe_sensor_{nullptr};
  binary_sensor::BinarySensor *swlock_sensor_{nullptr};
  std::vector<struct numbered_bsensor> balance_state_sensors_;

  bool update_{false};
  std::vector<uint8_t> data_;
  std::string version_;
  int string_count_{0};
  int ntc_count_{0};
};

}  // namespace jbd_bms
}  // namespace esphome
