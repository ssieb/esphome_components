#include "uart_mitm.h"
#include "esphome/core/log.h"

namespace esphome {
namespace serial {

static const char *const TAG = "uart_mitm";

void UARTMITM::loop() {
  uint8_t c;
  while (this->uart1_->available()) {
    this->uart1_->read_byte(&c);
    this->uart2_->write_byte(c);
  }
  while (this->uart2_->available()) {
    this->uart2_->read_byte(&c);
    this->uart1_->write_byte(c);
  }
}

void UARTMITM::dump_config() { ESP_LOGCONFIG(TAG, "UART MITM"); }

}  // namespace serial
}  // namespace esphome
