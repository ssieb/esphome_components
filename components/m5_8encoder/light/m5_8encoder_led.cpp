#include "m5_8encoder_led.h"
#include "esphome/core/log.h"

namespace esphome {
namespace m5_8encoder {

static const char *const TAG = "m5_8encoder.led";

light::LightTraits M5_8encoderLED::get_traits() {
  auto traits = light::LightTraits();
  traits.set_supported_color_modes({light::ColorMode::RGB});
  return traits;
}
void M5_8encoderLED::write_state(light::LightState *state) {
  float red, green, blue;
  state->current_values_as_rgb(&red, &green, &blue);
  this->parent_->set_led(this->number_, red * 255, green * 255, blue * 255);
}

}  // namespace m5_8encoder
}  // namespace esphome

