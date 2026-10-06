#pragma once

#include "esphome/components/light/addressable_light.h"
#include "esphome/components/light/light_output.h"
#include "../seesaw.h"

namespace esphome {
namespace seesaw {

class SeesawNeopixel : public light::AddressableLight {
 public:
  void setup() override;

  void write_state(light::LightState *state) override;

  int32_t size() const override { return this->num_leds_; }
  light::LightTraits get_traits() override {
    auto traits = light::LightTraits();
    traits.set_supported_color_modes({light::ColorMode::RGB});
    return traits;
  }

  void clear_effect_data() override { memset(this->effect_data_, 0x00, this->num_leds_); }

  void set_parent(Seesaw *parent) { this->parent_ = parent; }
  void set_pin(uint8_t pin) { this->pin_ = pin; }
  void set_num_leds(uint8_t num_leds) { this->num_leds_ = num_leds; }

 protected:
  light::ESPColorView get_view_internal(int32_t index) const override;

  Seesaw *parent_;
  uint8_t pin_;
  uint8_t num_leds_{1};
  // (G, R, B) per pixel, matching the byte order Seesaw expects on the wire.
  uint8_t *buf_{nullptr};
  uint8_t *effect_data_{nullptr};
};

}  // namespace seesaw
}  // namespace esphome
