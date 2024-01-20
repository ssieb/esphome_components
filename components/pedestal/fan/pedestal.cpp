#include "pedestal.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace pedestal {

static const char *const TAG = "pedestal.fan";

static const uint16_t TGL_POWER =  0b110110000001;
static const uint16_t TGL_OSC =    0b110110000000;
static const uint16_t SPEED_JUMP = 0b110110000100;
static const uint16_t SPEED_UP   = 0b110110010000;
static const uint16_t SPEED_DOWN = 0b110110100000;

void PedestalFan::setup() {
  this->traits_ = fan::FanTraits(true, true, false, 12);
  this->pulse_sensor_->add_on_state_callback([this](float state) {
    this->update_speed_(state);
  });
  this->osc_pin_->setup();
}

void PedestalFan::loop() {
  uint32_t now = millis();
  if (this->waiting_) {
    if (now < this->wait_until_)
      return;
    this->waiting_ = false;
  }
  if (!this->to_send_.empty()) {
    this->waiting_ = true;
    this->wait_until_ = now + 100;
    this->transmit_data_(this->to_send_.front());
    this->to_send_.pop_front();
    return;
  }
  if (this->changing_) {
    if (now - this->last_change_ < 1000)
      return;
    this->changing_ = false;
  }
  bool changed = false;
  if ((speed > 0) != this->state) {
    this->state = (speed > 0);
    changed = true;
  }
  if (speed != this->speed) {
    this->speed = speed;
    changed = true;
  }
  bool osc_state = this->osc_pin_->digital_read();
  if (osc_state != this->oscillating) {
    this->oscillating = osc_state;
    changed = true;
  }
  if (changed)
    this->publish_state();
}

void PedestalFan::dump_config() {
  LOG_FAN("", "Pedestal Fan", this);
  LOG_PIN("  Osc Pin: ", this->osc_pin_);
}

void PedestalFan::control(const fan::FanCall &call) {
  if (call.get_state().has_value()) {
    bool new_state = *call.get_state();
    if (this->state != new_state) {
      this->state = new_state;
      this->to_send_.push_back(TGL_POWER);
    }
  }
  if (call.get_oscillating().has_value()) {
    bool new_osc = *call.get_oscillating();
    if (this->oscillating != new_osc) {
      this->oscillating = new_osc;
      this->to_send_.push_back(TGL_OSC);
    }
  }
  if (call.get_speed().has_value()) {
    int new_speed = *call.get_speed();
    if (this->speed != new_speed) {
      this->changing_ = true;
      if (new_speed > this->speed) {
        for (int i = 0; i < new_speed - this->speed; i++)
          this->to_send_.push_back(SPEED_UP);
      } else {
        for (int i = 0; i < this->speed - new_speed ; i++)
          this->to_send_.push_back(SPEED_DOWN);
      }
      this->speed = new_speed;
    }
  }
  this->publish_state();
}

void PedestalFan::update_speed_(float value) {
  value = clamp(value, 0.0f, 100.0f);
  int speed;
  if (value < 5)
    speed = 0;
  else if (value < 13)
    speed = 1;
  else if (value < 20)
    speed = 2;
  else if (value < 28)
    speed = 3;
  else if (value < 35)
    speed = 4;
  else if (value < 42)
    speed = 5;
  else if (value < 49)
    speed = 6;
  else if (value < 56)
    speed = 7;
  else if (value < 63)
    speed = 8;
  else if (value < 70)
    speed = 9;
  else if (value < 78)
    speed = 10;
  else if (value < 90)
    speed = 11;
  else
    speed = 12;
  if (speed != this->current_speed_) {
    this->current_speed_ = speed;
    this->last_change_ = millis();
  }
}

void PedestalFan::transmit_data_(uint16_t msg) {
  ESP_LOGD(TAG, "transmitting code %02x", msg);
  auto transmit = this->transmitter_->transmit();
  auto *data = transmit.get_data();

  data->set_carrier_frequency(0);
  data->reserve(24);
  for (int b = 0x800; b; b >>= 1) {
    if (msg & b )
      data->item(1296, 406);
    else
      data->item(442, 1252);
  }
  transmit.perform();
}

}  // namespace pedestal
}  // namespace esphome
