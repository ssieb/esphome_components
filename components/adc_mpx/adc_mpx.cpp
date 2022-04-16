#include "adc_mpx.h"

#include "esphome/core/log.h"

namespace esphome {
namespace adc_mpx {

static const char *TAG = "adc_mpx";

void ADCMPXSensor::setup() {
  pin_->pin_mode(gpio::FLAG_INPUT);
}

void ADCMPXSensor::dump_config() {
  LOG_SENSOR("", "ADC Multiplexor Sensor", this);
  LOG_PIN("  Pin: ", pin_);
}

void ADCMPXSensor::update() {
  pin_->pin_mode(gpio::FLAG_OUTPUT);
  pin_->digital_write(true);
  float value = source_->sample();
  pin_->pin_mode(gpio::FLAG_INPUT);
  publish_state(value);
}

}  // namespace adc_mpx
}  // namespace esphome
