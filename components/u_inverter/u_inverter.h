#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace u_inverter {

enum UInverterCmd : uint8_t {
  CMD_NONE = 0,
  CMD_HSTS,
  CMD_HIMSG1,
  CMD_HGRID,
  CMD_HOP,
  CMD_HBAT,
  CMD_HPV,
  CMD_HTEMP,
  CMD_HEEP,
  CMD_HBMS,
  CMD_HGEN,
};

class UInverterListener {
 public:
  virtual void handle_message(UInverterCmd cmd, std::string &message) = 0;
};

class UInverter : public uart::UARTDevice, public PollingComponent {
 public:
  void dump_config() override;
  void setup() override;
  void loop() override;
  void update() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void register_listener(UInverterListener *listener) { this->listeners_.push_back(listener); }
  void send_command(UInverterCmd cmd);

 protected:
  int state_{0};
  std::vector<uint8_t> buffer_;
  std::vector<UInverterCmd> queue_;
  uint32_t last_data_time_;
  std::string version_;
  std::vector<UInverterListener *> listeners_{};

  void handle_response_();
  void check_queue_();
};

}  // namespace u_inverter
}  // namespace esphome
