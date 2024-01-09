#pragma once

#include "esphome/components/key_provider/key_provider.h"
#include "esphome/components/voltage_sampler/voltage_sampler.h"
#include "esphome/core/component.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace analog_keypad {

struct KeyInfo {
  float voltage;
  float variance;
  uint8_t key;
};

class AnalogKeypadListener {
 public:
  virtual void button_pressed(int index){};
  virtual void button_released(int index){};
  virtual void key_pressed(uint8_t key){};
  virtual void key_released(uint8_t key){};
};

class AnalogKeypad : public key_provider::KeyProvider, public Component {
 public:
  void loop() override;
  void dump_config() override;
  void set_source(voltage_sampler::VoltageSampler *source) { source_ = source; }
  void set_debounce_time(int debounce_time) { debounce_time_ = debounce_time; };
  void add_key(float voltage, float variance, std::string key);
  void add_key(float voltage, float variance, uint8_t key = 0);

  void register_listener(AnalogKeypadListener *listener);

 protected:
  voltage_sampler::VoltageSampler *source_;
  std::vector<KeyInfo *> keys_;
  int debounce_time_ = 0;
  int pressed_key_ = -1;

  std::vector<AnalogKeypadListener *> listeners_{};
};

}  // namespace analog_keypad
}  // namespace esphome
