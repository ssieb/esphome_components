#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/fan/fan.h"
#include "esphome/components/remote_transmitter/remote_transmitter.h"
#include "esphome/components/sensor/sensor.h"

#include <deque>

namespace esphome {
namespace pedestal {

struct DutyCycleStore {
  volatile uint32_t last_interrupt{0};
  volatile uint32_t on_time{0};
  volatile bool last_level{false};
  ISRInternalGPIOPin pin;

  static void gpio_intr(DutyCycleStore *arg);
};

class PedestalFan : public Component, public fan::Fan, public remote_base::RemoteTransmittable {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;
  fan::FanTraits get_traits() override { return this->traits_; }
  void set_osc_pin(GPIOPin *pin) { this->osc_pin_ = pin; }
  void set_speed_pin(InternalGPIOPin *pin) { this->speed_pin_ = pin; }

 protected:
  void control(const fan::FanCall &call) override;
  void write_state_();
  void transmit_data_(uint16_t msg);

  fan::FanTraits traits_;
 
  sensor::Sensor *pulse_sensor_;
  GPIOPin *osc_pin_;
  InternalGPIOPin *speed_pin_;
  DutyCycleStore store_{};
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
