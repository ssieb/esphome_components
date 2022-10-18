#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace vbus {

class VBus : public uart::UARTDevice, public Component {
 public:

  void dump_config() override;
  void loop() override;

  float get_setup_priority() const { return setup_priority::DATA; }

  void add_message_callback(std::function<void(uint16_t, uint16_t, uint16_t, std::vector<uint8_t>)> &&callback);

 protected:
  int state_{0};
  std::vector<uint8_t> buffer_;
  uint8_t protocol_;
  uint16_t source_;
  uint16_t dest_;
  uint16_t command_;
  uint8_t frames_;
  uint8_t cframe_;
  uint8_t fbytes_[6];
  int fbcount_;

  CallbackManager<void(uint16_t, uint16_t, uint16_t, std::vector<uint8_t>)> message_callback_{};
};

}  // namespace vbus
}  // namespace esphome
