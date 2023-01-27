#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace jsdrive {

class JSDrive : public Component {
 public:
  float get_setup_priority() const override { return setup_priority::LATE; }
  void loop() override;
  void dump_config() override;
  void set_remote_uart(uart::UARTComponent *uart) { this->remote_uart_ = uart; }
  void set_desk_uart(uart::UARTComponent *uart) { this->desk_uart_ = uart; }
  void set_height_sensor(sensor::Sensor *sensor) { height_sensor_ = sensor; }
  void set_up_bsensor(binary_sensor::BinarySensor *sensor) { up_bsensor_ = sensor; }
  void set_down_bsensor(binary_sensor::BinarySensor *sensor) { down_bsensor_ = sensor; }
  void set_memory1_bsensor(binary_sensor::BinarySensor *sensor) { memory1_bsensor_ = sensor; }
  void set_memory2_bsensor(binary_sensor::BinarySensor *sensor) { memory2_bsensor_ = sensor; }
  void set_memory3_bsensor(binary_sensor::BinarySensor *sensor) { memory3_bsensor_ = sensor; }

 protected:
  uart::UARTComponent *remote_uart_{nullptr};
  uart::UARTComponent *desk_uart_{nullptr};
  sensor::Sensor *height_sensor_{nullptr};
  binary_sensor::BinarySensor *up_bsensor_{nullptr};
  binary_sensor::BinarySensor *down_bsensor_{nullptr};
  binary_sensor::BinarySensor *memory1_bsensor_{nullptr};
  binary_sensor::BinarySensor *memory2_bsensor_{nullptr};
  binary_sensor::BinarySensor *memory3_bsensor_{nullptr};

  std::vector<uint8_t> rem_buffer_;
  std::vector<uint8_t> desk_buffer_;
  bool rem_rx_{false};
  bool desk_rx_{false};
};

}  // namespace jsdrive
}  // namespace esphome
