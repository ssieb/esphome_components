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

static const uint8_t duty_to_speed[13] = {5, 13, 20, 28, 35, 42, 49, 56, 63, 70, 78, 90, 100};

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
  int speed = this->measured_speed_;
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
  bool changed = false;
  if (call.get_state().has_value()) {
    bool new_state = *call.get_state();
    if (this->state != new_state) {
      this->state = new_state;
      this->to_send_.push_back(TGL_POWER);
      changed = true;
    }
  }
  if (call.get_speed().has_value() && !changed) {
    int new_speed = *call.get_speed();
    if (this->speed != new_speed) {
      if (new_speed > this->speed) {
        for (int i = 0; i < new_speed - this->speed; i++)
          this->to_send_.push_back(SPEED_UP);
      } else {
        for (int i = 0; i < this->speed - new_speed ; i++)
          this->to_send_.push_back(SPEED_DOWN);
      }
      this->speed = new_speed;
      changed = true;
    }
  }
  if (call.get_oscillating().has_value()) {
    bool new_osc = *call.get_oscillating();
    if (this->oscillating != new_osc) {
      this->oscillating = new_osc;
      this->to_send_.push_back(TGL_OSC);
      changed = true;
    }
  }
  if (changed) {
    this->changing_ = true;
    this->last_change_ = millis();
    this->publish_state();
  }
}

void PedestalFan::update_speed_(float value) {
  value = clamp(value, 0.0f, 100.0f);
  int speed;
  for (speed = 0; value > duty_to_speed[speed]; speed++);
  ESP_LOGD(TAG, "detected speed is %d", speed);
  if (speed != this->measured_speed_) {
    this->measured_speed_ = speed;
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
