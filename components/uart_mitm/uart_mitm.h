#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace serial {

class UARTMITM : public Component {
 public:
  float get_setup_priority() const override { return setup_priority::LATE; }
  void loop() override;
  void dump_config() override;
  void set_uart1(uart::UARTComponent *uart) { this->uart1_ = uart; }
  void set_uart2(uart::UARTComponent *uart) { this->uart2_ = uart; }

 protected:
  uart::UARTComponent *uart1_;
  uart::UARTComponent *uart2_;
};

}  // namespace serial
}  // namespace esphome
