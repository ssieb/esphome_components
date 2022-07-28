#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome {
namespace wiegand {

struct WiegandStore {
  ISRInternalGPIOPin d0;
  ISRInternalGPIOPin d1;
  volatile uint32_t value{0};
  volatile uint32_t last_bit_time{0};
  volatile bool done{true};
  volatile uint8_t count{0};

  static void d0_gpio_intr(WiegandStore *arg);
  static void d1_gpio_intr(WiegandStore *arg);
};

class WiegandTextSensor : public Component,  public text_sensor::TextSensor {
 public:
  float get_setup_priority() const override { return setup_priority::HARDWARE; }
  void setup() override;
  void loop() override;
  void dump_config() override;

  void set_d0_pin(InternalGPIOPin *pin) { this->d0_pin_ = pin; };
  void set_d1_pin(InternalGPIOPin *pin) { this->d1_pin_ = pin; };

 protected:
  InternalGPIOPin *d0_pin_;
  InternalGPIOPin *d1_pin_;
  WiegandStore store_{};
};

}  // namespace wiegand
}  // namespace esphome
