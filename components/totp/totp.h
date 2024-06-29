#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/time/real_time_clock.h"

namespace esphome {
namespace totp {

class TOTPComponent : public Component {
 public:
  void set_secret(const std::string &secret) { this->secret_ = secret; }
  void set_time(time::RealTimeClock *time) { this->time_ = time; }
  void set_totp(text_sensor::TextSensor *totp) { this->totp_ = totp; }
  void set_countdown(sensor::Sensor *countdown) { this->countdown_ = countdown; }
  void loop() override;
  std::string get_current_totp();
  int get_countdown() const;
  uint32_t get_next_update_time() const;

 protected:
  std::string secret_;
  time::RealTimeClock *time_;
  text_sensor::TextSensor *totp_{nullptr};
  sensor::Sensor *countdown_{nullptr};
  bool init_{false};
  int last_second_{0};
  uint32_t next_time_step_{0};
  std::string last_totp_{"000000"};

  std::string generate_totp_(ESPTime &now);
};

}  // namespace totp
}  // namespace esphome
