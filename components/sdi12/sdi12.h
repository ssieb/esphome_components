#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace sdi12 {

class SDI12;

class SDI12Listener {
 public:
  void set_address(uint8_t address) { this->address_ = address; }

  void on_values(uint8_t address, std::vector<float> &values);

 protected:
  uint8_t address_{0};

  virtual void handle_values(std::vector<float> &values) = 0;
};

class SDI12 : public uart::UARTDevice, public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void register_listener(SDI12Listener *listener) { this->listeners_.push_back(listener); }

  void start_measurement(uint8_t address);

 protected:
  int state_{0};
  int phase_{0};
  uint8_t address_;
  uint8_t dataset_;
  uint8_t mexpect_;
  std::vector<uint8_t> buffer_;
  std::vector<float> values_;
  bool scanning_{false};
  bool waiting_{false};
  uint8_t cur_addr_;
  uint8_t tries_{0};
  uint32_t start_;
  uint32_t timeout_;
  std::vector<uint8_t> found_;

  std::vector<SDI12Listener *> listeners_{};
};

}  // namespace sdi12
}  // namespace esphome
