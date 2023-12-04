#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace m5_8encoder {

class M5_8encoder;

class EncoderListener {
 public:
  virtual void on_update(int32_t value) = 0;
  uint8_t get_index() { return this->index_; }
 protected:
  uint8_t index_;
};

class ToggleListener {
 public:
  virtual void on_update(bool state) = 0;
};

class M5_8encoder : public i2c::I2CDevice, public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;

  float get_setup_priority() const override { return setup_priority::DATA; }

  void register_encoder_listener(EncoderListener *listener) { this->encoder_listeners_.push_back(listener); }
  void register_toggle_listener(ToggleListener *listener) { this->toggle_listeners_.push_back(listener); }

  void set_led(uint8_t led, uint8_t red, uint8_t green, uint8_t blue);

 protected:
  std::vector<EncoderListener *> encoder_listeners_{};
  std::vector<ToggleListener *> toggle_listeners_{};
  uint8_t version_{0};
};

}  // namespace m5_8encoder
}  // namespace esphome
