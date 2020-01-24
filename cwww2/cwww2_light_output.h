#pragma once

#include "esphome/core/component.h"
#include "esphome/components/output/float_output.h"
#include "esphome/components/light/light_output.h"

namespace esphome {
namespace cwww2 {

class CWWW2LightOutput : public light::LightOutput {
 public:
  void set_color_temperature_output(output::FloatOutput *color_temperature) { color_temperature_ = color_temperature; }
  void set_brightness_output(output::FloatOutput *brightness_output) { brightness_output_ = brightness_output; }
  void set_cold_white_temperature(float cold_white_temperature) { cold_white_temperature_ = cold_white_temperature; }
  void set_warm_white_temperature(float warm_white_temperature) { warm_white_temperature_ = warm_white_temperature; }
  light::LightTraits get_traits() override {
    auto traits = light::LightTraits();
    traits.set_supports_brightness(true);
    traits.set_supports_rgb(false);
    traits.set_supports_rgb_white_value(false);
    traits.set_supports_color_temperature(true);
    traits.set_min_mireds(this->cold_white_temperature_);
    traits.set_max_mireds(this->warm_white_temperature_);
    return traits;
  }
  void write_state(light::LightState *state) override {
    float temperature = clamp(state->current_values.get_color_temperature(),
                              cold_white_temperature_, warm_white_temperature_);
    float ratio = (temperature - cold_white_temperature_) / (warm_white_temperature_ - cold_white_temperature_);
    ESP_LOGD("cwww2", "color temperature %f", ratio);
    this->color_temperature_->set_level(ratio);
    float bright;
    state->current_values_as_brightness(&bright);
    ESP_LOGD("cwww2", "brightness %f", bright);
    this->brightness_output_->set_level(bright);
  }

 protected:
  output::FloatOutput *color_temperature_;
  output::FloatOutput *brightness_output_;
  float cold_white_temperature_;
  float warm_white_temperature_;
};

}  // namespace cwww2
}  // namespace esphome
