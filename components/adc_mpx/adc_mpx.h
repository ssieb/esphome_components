#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/voltage_sampler/voltage_sampler.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace adc_mpx {

class ADCMPXSensor : public sensor::Sensor, public PollingComponent {
 public:
  void setup() override;
  void update() override;
  void dump_config() override;
  float get_setup_priority() const override {
    // After the base sensor has been initialized
    return setup_priority::DATA - 1.0f;
  }

  void set_source(voltage_sampler::VoltageSampler *source) { source_ = source; }
  void set_pin(GPIOPin *pin) { pin_ = pin; }

 protected:
  GPIOPin *pin_;
  voltage_sampler::VoltageSampler *source_;
};

}  // namespace adc_mpx
}  // namespace esphome
