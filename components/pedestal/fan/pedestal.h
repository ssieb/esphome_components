#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/fan/fan.h"
#include "esphome/components/remote_transmitter/remote_transmitter.h"
#include "esphome/components/sensor/sensor.h"

#include <deque>

namespace esphome {
namespace pedestal {

class PedestalFan : public Component, public fan::Fan, public remote_base::RemoteTransmittable {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;
  fan::FanTraits get_traits() override { return this->traits_; }
  void set_pulse_sensor(sensor::Sensor *sens) { this->pulse_sensor_ = sens; }
  void set_osc_pin(GPIOPin *pin) { this->osc_pin_ = pin; }

 protected:
  void control(const fan::FanCall &call) override;
  void write_state_();
  void update_speed_(float value);
  void transmit_data_(uint16_t msg);

  fan::FanTraits traits_;
 
  sensor::Sensor *pulse_sensor_;
  GPIOPin *osc_pin_;
  int measured_speed_{0};
  bool changing_{false};
  uint32_t last_change_;

  // IR queue
  std::deque<uint16_t> to_send_;
  bool waiting_{false};
  uint32_t wait_until_{0};
};

}  // namespace pedestal
}  // namespace esphome
