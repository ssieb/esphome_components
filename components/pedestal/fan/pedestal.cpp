#include "pedestal.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace pedestal {

static const char *const TAG = "pedestal.fan";

static const uint16_t TGL_POWER =  0b110110000001;
static const uint16_t TGL_OSC =    0b110110001000;
static const uint16_t SPEED_JUMP = 0b110110000100;
static const uint16_t SPEED_UP   = 0b110110010000;
static const uint16_t SPEED_DOWN = 0b110110100000;

static const uint8_t duty_to_speed[13] = {5, 20, 28, 35, 42, 49, 56, 63, 70, 78, 87, 95, 100};

void PedestalFan::setup() {
  this->traits_ = fan::FanTraits(true, true, false, 12);
  this->osc_pin_->setup();
  this->store_.pin = this->speed_pin_->to_isr();
  this->store_.last_level = this->speed_pin_->digital_read();
  this->speed_pin_->attach_interrupt(DutyCycleStore::gpio_intr, &this->store_, gpio::INTERRUPT_ANY_EDGE);
}

void PedestalFan::loop() {
  uint32_t now = millis();
  if (this->last_speed_check_ == 0) {
    this->last_speed_check_ = now;
  } else if (now - this->last_speed_check_ > 15) {
    this->last_speed_check_ = now;
    uint32_t count, total_on_time, total_time;
    {
      InterruptLock lock;
      count = this->store_.count;
      if (count == 0)
        this->store_.stopped = true;
      this->store_.count = 0;
      total_on_time = this->store_.total_on_time;
      total_time = this->store_.total_time;
    }
    if (count == 0) {
      this->measured_speed_ = this->speed_pin_->digital_read() ? 12 : 0;
    } else {
      int value = total_on_time * 100 / total_time;
      int speed = 0;
      while (value > duty_to_speed[speed])
        speed++;
      if (speed != this->measured_speed_) {
        ESP_LOGD(TAG, "detected speed is %d", speed);
        this->measured_speed_ = speed;
        this->last_change_ = millis();
      }
    }
  }

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

void IRAM_ATTR DutyCycleStore::gpio_intr(DutyCycleStore *arg) {
  const bool new_level = arg->pin.digital_read();
  if (new_level == arg->last_level)
    return;
  arg->last_level = new_level;
  const uint32_t now = micros();

  if (arg->stopped) {
    arg->start_time = 0;
    arg->on_time = 0;
    arg->total_time = 0;
    arg->total_on_time = 0;
    arg->stopped = false;
  }

  if (new_level) {
    if (arg->on_time > 0) {
      arg->total_on_time += arg->on_time;
      arg->on_time = 0;
      arg->total_time += now - arg->start_time;
      arg->count++;
    }
    arg->start_time = now;
  } else {
    if (arg->start_time > 0)
      arg->on_time = now - arg->start_time;
  }
}

}  // namespace pedestal
}  // namespace esphome
