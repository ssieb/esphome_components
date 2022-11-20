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
  void set_boiler_temp_sensor(sensor::Sensor *sensor) { this->boiler_temp_sensor_ = sensor; }
  void set_return_temp_sensor(sensor::Sensor *sensor) { this->return_temp_sensor_ = sensor; }
  void set_buffer_below_temp_sensor(sensor::Sensor *sensor) { this->buffer_below_temp_sensor_ = sensor; }
  void set_buffer_middle_temp_sensor(sensor::Sensor *sensor) { this->buffer_middle_temp_sensor_ = sensor; }
  void set_buffer_above_temp_sensor(sensor::Sensor *sensor) { this->buffer_above_temp_sensor_ = sensor; }

 protected:
  uint8_t update_interval_{0};
  sensor::Sensor *boiler_temp_sensor_{nullptr};
  sensor::Sensor *return_temp_sensor_{nullptr};
  sensor::Sensor *buffer_below_temp_sensor_{nullptr};
  sensor::Sensor *buffer_middle_temp_sensor_{nullptr};
  sensor::Sensor *buffer_above_temp_sensor_{nullptr};

  bool reading_{false};
  std::vector<uint8_t> buffer_;
  void handle_data_(uint8_t *data);
};

}  // namespace eta_sh
}  // namespace esphome
