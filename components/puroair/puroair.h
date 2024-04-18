#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace puroair {

class PuroAir : public Component {
 public:
  float get_setup_priority() const override { return setup_priority::LATE; }
  void loop() override;
  void dump_config() override;
  void set_button_uart(uart::UARTComponent *uart) { this->button_uart_ = uart; }
  void set_control_uart(uart::UARTComponent *uart) { this->control_uart_ = uart; }

  void press_button(uint8_t button) { this->buttons_.push_back(button); }

 protected:
  uart::UARTComponent *button_uart_{nullptr};
  uart::UARTComponent *control_uart_{nullptr};

  std::vector<uint8_t> control_buffer_ = std::vector<uint8_t>(3);
  bool button_rx_{false};
  bool control_rx_{false};
  uint8_t bstate_{0};
  uint8_t new_bstate_{0xff};
  uint32_t cstate_{0};
  uint32_t last_send_{0};
  bool sending_{false};
  std::vector<uint8_t> buttons_;
};

}  // namespace puroair
}  // namespace esphome
