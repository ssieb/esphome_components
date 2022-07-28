#include "wiegand.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace wiegand {

static const char *TAG = "wiegand.text_sensor";

void IRAM_ATTR HOT WiegandStore::d0_gpio_intr(WiegandStore *arg) {
  arg->count++;
  arg->value <<= 1;
  arg->last_bit_time = millis();
  arg->done = false;
}

void IRAM_ATTR HOT WiegandStore::d1_gpio_intr(WiegandStore *arg) {
  arg->count++;
  arg->value = (arg->value << 1) | 1;
  arg->last_bit_time = millis();
  arg->done = false;
}

void WiegandTextSensor::setup() {
  this->d0_pin_->setup();
  this->store_.d0 = this->d0_pin_->to_isr();
  this->d1_pin_->setup();
  this->store_.d1 = this->d1_pin_->to_isr();
  this->d0_pin_->attach_interrupt(WiegandStore::d0_gpio_intr, &this->store_, gpio::INTERRUPT_FALLING_EDGE);
  this->d1_pin_->attach_interrupt(WiegandStore::d1_gpio_intr, &this->store_, gpio::INTERRUPT_FALLING_EDGE);
}

void WiegandTextSensor::loop() {
  if (this->store_.done)
    return;
  if (millis() - this->store_.last_bit_time < 100)
    return;
  uint8_t count = this->store_.count;
  uint32_t value = this->store_.value;
  this->store_.count = 0;
  this->store_.value = 0;
  this->store_.done = true;
  if (count != 26) {
    ESP_LOGE(TAG, "received %d bits instead of 26", count);
    return;
  }
  this->publish_state(to_string((value >> 1) & 0xffffff));
}

void WiegandTextSensor::dump_config() { LOG_TEXT_SENSOR("", "Wiegand Text Sensor", this); }

}  // namespace wiegand
}  // namespace esphome
