#pragma once

#include "esphome/core/component.h"
#include "esphome/components/key_provider/key_provider.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace honeywell {

class HoneyWell : public Component, public key_provider::KeyProvider, public uart::UARTDevice {
 public:
  float get_setup_priority() const override { return setup_priority::LATE; }
  void loop() override;
  void dump_config() override;

 protected:
  std::vector<uint8_t> rx_message_;
};

}  // namespace honeywell
}  // namespace esphome
