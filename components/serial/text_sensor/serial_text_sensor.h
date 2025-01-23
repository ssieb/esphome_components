#pragma once

#include "esphome/core/component.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace serial {

class SerialTextSensor : public Component,  public text_sensor::TextSensor, public uart::UARTDevice {
 public:
  float get_setup_priority() const override { return setup_priority::LATE; }
  void loop() override;
  void dump_config() override;

 protected:
  void handle_char_(uint8_t c);
  void publish_();
  std::vector<uint8_t> rx_message_;
  bool have_return_{false};
};

}  // namespace serial
}  // namespace esphome
