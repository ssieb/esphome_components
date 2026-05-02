#include "magic_switch.h"
#include "esphome/core/log.h"

namespace esphome {
namespace magic_switch {

static const char *const TAG = "magic_switch";

void MagicSwitch::setup() {
  this->pin_->setup();
  this->isr_pin_ = this->pin_->to_isr();
  this->pin_->attach_interrupt(MagicSwitch::edge_intr, this, gpio::INTERRUPT_ANY_EDGE);
  this->last_state_ = isr_pin_.digital_read();
}

void MagicSwitch::loop() {
  uint32_t pulse = this->pulse_;
  this->pulse_ = 0;
  if (pulse) {
    ESP_LOGD(TAG, "detected pulse of %uus", pulse);
    this->switch_trigger_->trigger();
  }
}

float MagicSwitch::get_setup_priority() const { return setup_priority::DATA; }

void MagicSwitch::dump_config() {
  ESP_LOGCONFIG(TAG, "");
  LOG_PIN("  Pin: ", this->pin_);
}

void IRAM_ATTR HOT MagicSwitch::edge_intr(MagicSwitch *comp) {
  bool state = comp->isr_pin_.digital_read();
  if (state != comp->last_state_) {
    comp->last_state_ = state;
    //only triggered at EDGE
    uint32_t now = micros();
    if (state) {
      //Rising Edge
      comp->pulse_start_ = now;
    } else {
      //Falling Edge
      uint32_t diff = now - comp->pulse_start_;
      if (diff > comp->timeout_)
        comp->pulse_ = diff;
    }
  }
}

}  // namespace magic_switch
}  // namespace esphome
