#pragma once

#include "esphome.h"
#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace apc {

class ApcSmartUpsSerial : public PollingComponent, public uart::UARTDevice {
 public:
  void dump_config() override;
  void setup() override;
  void loop() override;
  void update() override;

  void set_internal_temp_sensor(sensor::Sensor *internal_temp_sensor) { internal_temp_sensor_ = internal_temp_sensor; }
  void set_battery_volt_sensor(sensor::Sensor *battery_volt_sensor) { battery_volt_sensor_ = battery_volt_sensor; }
  void set_battery_level_sensor(sensor::Sensor *battery_level_sensor) { battery_level_sensor_ = battery_level_sensor; }
  void set_power_load_sensor(sensor::Sensor *power_load_sensor) { power_load_sensor_ = power_load_sensor; }
  void set_input_volt_sensor(sensor::Sensor *input_volt_sensor) { input_volt_sensor_ = input_volt_sensor; }
  void set_output_volt_sensor(sensor::Sensor *output_volt_sensor) { output_volt_sensor_ = output_volt_sensor; }
  void set_line_freq_sensor(sensor::Sensor *line_freq_sensor) { line_freq_sensor_ = line_freq_sensor; }
  void set_est_runtime_sensor(sensor::Sensor *est_runtime_sensor) { est_runtime_sensor_ = est_runtime_sensor; }

 protected:
  void handle_setup_line_();
  void handle_line_();
  void handle_int_(uint8_t);

  bool in_setup_;
  int state_;
  unsigned long cmd_start_;
  std::string line_;
  std::string model_;
  std::string firmware_ver_;
  std::string serial_num_;

  sensor::Sensor *internal_temp_sensor_;          // C    000.0     °C
  sensor::Sensor *battery_volt_sensor_;           // B    00.00     V
  sensor::Sensor *battery_level_sensor_;          // f    000.0     %
  sensor::Sensor *power_load_sensor_;             // P    000.0     %
  sensor::Sensor *input_volt_sensor_;             // L    000.0     V
  sensor::Sensor *output_volt_sensor_;            // O    000.0     V
  sensor::Sensor *line_freq_sensor_;              // F    00.00     Hz
  sensor::Sensor *est_runtime_sensor_;            // j    0000:     Minutes
  // Sensor *smart_mode_online_sensor_;      // Y    SM
  // Sensor *cause_transf_sensor_;           // G    R, H, L, T, O, S
  // Sensor *selftest_result_sensor_;        // X    OK, BT, NG, NO
  // Sensor *ups_status_sensor_;             // Q    08, 10, 50

};

}  // namespace apc
}  // namespace esphome

