#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace desky {

enum DeskyOperation : uint8_t {
  DESKY_OPERATION_IDLE = 0,
  DESKY_OPERATION_RAISING,
  DESKY_OPERATION_LOWERING,
};

const char *desky_operation_to_str(DeskyOperation op);

class Desky : public Component,  public sensor::Sensor, public uart::UARTDevice {
 public:
  float get_setup_priority() const override { return setup_priority::LATE; }
  void setup() override;
  void loop() override;
  void dump_config() override;

  void set_height_sensor(sensor::Sensor *sensor) { this->height_sensor_ = sensor; }
  void set_up_pin(GPIOPin *pin) { this->up_pin_ = pin; }
  void set_down_pin(GPIOPin *pin) { this->down_pin_ = pin; }
  void set_request_pin(GPIOPin *pin) { this->request_pin_ = pin; }
  void set_up_in_pin(GPIOPin *pin) { this->up_in_pin_ = pin; }
  void set_down_in_pin(GPIOPin *pin) { this->down_in_pin_ = pin; }
  void set_stopping_distance(int distance) { this->stopping_distance_ = distance; }
  void set_timeout(int timeout) { this->timeout_ = timeout; }
  void set_intercept(bool intercept) { this->intercept_ = intercept; }

  void move_to(int height);
  void stop();

  DeskyOperation current_operation{DESKY_OPERATION_IDLE};

 protected:
  sensor::Sensor *height_sensor_{nullptr};
  GPIOPin *up_pin_{nullptr};
  GPIOPin *down_pin_{nullptr};
  GPIOPin *request_pin_{nullptr};
  GPIOPin *up_in_pin_{nullptr};
  GPIOPin *down_in_pin_{nullptr};
  int stopping_distance_;
  int state_{0};
  uint8_t protocol_{0};
  int current_pos_{0};
  int target_pos_{-1};
  int timeout_{-1};
  bool intercept_{false};
  bool error_{false};
  uint64_t start_time_;
  uint64_t request_time_{0};
  uint64_t error_time_{0};
  std::vector<uint8_t> rx_data_;
};

}  // namespace desky
}  // namespace esphome
