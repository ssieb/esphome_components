#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/automation.h"

namespace esphome {
namespace ifan04 {

class IFan04 : public Component, public uart::UARTDevice {
 public:
  void dump_config() override;
  void loop() override;

  Trigger<int> *get_fan_trigger() const { return fan_trigger_; }
  Trigger<> *get_light_trigger() const { return light_trigger_; }
  Trigger<> *get_buzzer_trigger() const { return buzzer_trigger_; }

 protected:
  void handle_char_(uint8_t c);
  void handle_command_(uint8_t type, uint8_t param);

  Trigger<int> *fan_trigger_ = new Trigger<int>();
  Trigger<> *light_trigger_ = new Trigger<>();
  Trigger<> *buzzer_trigger_ = new Trigger<>();
};

}  // namespace ifan04
}  // namespace esphome

