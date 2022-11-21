#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace eta_sh {

class ETA_SH : public Component,  public uart::UARTDevice {
 public:
  float get_setup_priority() const override { return setup_priority::LATE; }
  void setup() override;
  void loop() override;
  void dump_config() override;

  void set_update_interval(uint8_t interval) { this->update_interval_ = interval; }
  void set_fan_speed_sensor(sensor::Sensor *sensor) { this->fan_speed_sensor_ = sensor; }
  void set_boiler_temp_sensor(sensor::Sensor *sensor) { this->boiler_temp_sensor_ = sensor; }
  void set_return_temp_sensor(sensor::Sensor *sensor) { this->return_temp_sensor_ = sensor; }
  void set_buffer_bottom_temp_sensor(sensor::Sensor *sensor) { this->buffer_bottom_temp_sensor_ = sensor; }
  void set_buffer_middle_temp_sensor(sensor::Sensor *sensor) { this->buffer_middle_temp_sensor_ = sensor; }
  void set_buffer_top_temp_sensor(sensor::Sensor *sensor) { this->buffer_top_temp_sensor_ = sensor; }
  void set_exhaust_temp_sensor(sensor::Sensor *sensor) { this->exhaust_temp_sensor_ = sensor; }
  void set_oxygen_sensor(sensor::Sensor *sensor) { this->oxygen_sensor_ = sensor; }
  void set_outside_temp_sensor(sensor::Sensor *sensor) { this->outside_temp_sensor_ = sensor; }
  void set_buffer_load_sensor(sensor::Sensor *sensor) { this->buffer_load_sensor_ = sensor; }
  void set_room1_temp_sensor(sensor::Sensor *sensor) { this->room1_temp_sensor_ = sensor; }
  void set_room1_output_temp_sensor(sensor::Sensor *sensor) { this->room1_output_temp_sensor_ = sensor; }
  void set_external_heater_temp_sensor(sensor::Sensor *sensor) { this->external_heater_temp_sensor_ = sensor; }

 protected:
  uint8_t update_interval_{0};
  sensor::Sensor *fan_speed_sensor_{nullptr};
  sensor::Sensor *boiler_temp_sensor_{nullptr};
  sensor::Sensor *return_temp_sensor_{nullptr};
  sensor::Sensor *buffer_bottom_temp_sensor_{nullptr};
  sensor::Sensor *buffer_middle_temp_sensor_{nullptr};
  sensor::Sensor *buffer_top_temp_sensor_{nullptr};
  sensor::Sensor *exhaust_temp_sensor_{nullptr};
  sensor::Sensor *oxygen_sensor_{nullptr};
  sensor::Sensor *outside_temp_sensor_{nullptr};
  sensor::Sensor *buffer_load_sensor_{nullptr};
  sensor::Sensor *room1_temp_sensor_{nullptr};
  sensor::Sensor *room1_output_temp_sensor_{nullptr};
  sensor::Sensor *external_heater_temp_sensor_{nullptr};

  bool reading_{false};
  std::vector<uint8_t> buffer_;
  void handle_data_(uint8_t *data);
};

}  // namespace eta_sh
}  // namespace esphome
