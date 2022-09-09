#include "wiegand.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace wiegand {

static const char *TAG = "wiegand.text_sensor";

void IRAM_ATTR HOT WiegandStore::d0_gpio_intr(WiegandStore *arg) {
  if (arg->d0.digital_read())
    return;
  arg->count++;
  arg->value <<= 1;
  arg->last_bit_time = millis();
  arg->done = false;
}

void IRAM_ATTR HOT WiegandStore::d1_gpio_intr(WiegandStore *arg) {
  if (arg->d1.digital_read())
    return;
  arg->count++;
  arg->value = (arg->value << 1) | 1;
  arg->last_bit_time = millis();
  arg->done = false;
}

void Wiegand::setup() {
  this->d0_pin_->setup();
  this->store_.d0 = this->d0_pin_->to_isr();
  this->d1_pin_->setup();
  this->store_.d1 = this->d1_pin_->to_isr();
  this->d0_pin_->attach_interrupt(WiegandStore::d0_gpio_intr, &this->store_, gpio::INTERRUPT_FALLING_EDGE);
  this->d1_pin_->attach_interrupt(WiegandStore::d1_gpio_intr, &this->store_, gpio::INTERRUPT_FALLING_EDGE);
}

void Wiegand::loop() {
  if (this->store_.done)
    return;
  if (millis() - this->store_.last_bit_time < 100)
    return;
  uint8_t count = this->store_.count;
  uint64_t value = this->store_.value;
  this->store_.count = 0;
  this->store_.value = 0;
  this->store_.done = true;
  if (count == 26) {
    std::string tag = to_string((value >> 1) & 0xffffff);
    ESP_LOGD(TAG, "received 26-bit tag: %s", tag.c_str());
    for (auto *trigger : this->tag_triggers_)
      trigger->trigger(tag);
  } else if (count == 34) {
    std::string tag = to_string((value >> 1) & 0xffffffff);
    ESP_LOGD(TAG, "received 34-bit tag: %s", tag.c_str());
    for (auto *trigger : this->tag_triggers_)
      trigger->trigger(tag);
  } else {
    ESP_LOGD(TAG, "received %d-bit value: %llx", count, value);
  }
}

void Wiegand::dump_config() {
  ESP_LOGCONFIG(TAG, "Wiegand reader:");
  LOG_PIN("  D0 pin: ", this->d0_pin_);
  LOG_PIN("  D1 pin: ", this->d1_pin_);
}

}  // namespace wiegand
}  // namespace esphome
