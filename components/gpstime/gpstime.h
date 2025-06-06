#pragma once

#include "esphome/components/time/real_time_clock.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace gpstime {

class GPSTime : public time::RealTimeClock, public uart::UARTDevice {
 public:
  float get_setup_priority() const override { return setup_priority::LATE; }
  void loop() override;
  void update() override;
  void dump_config() override;

 protected:
  bool receiving_{false};
  bool ending_{false};
  void handle_message_();
  std::vector<uint8_t> rx_buffer_;
  bool have_time_{false};
};

}  // namespace gpstime
}  // namespace esphome
