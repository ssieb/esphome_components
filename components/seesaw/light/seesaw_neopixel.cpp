#include "seesaw_neopixel.h"
#include "esphome/core/log.h"

namespace esphome {
namespace seesaw {

static const char *const TAG = "seesaw.neopixel";

void SeesawNeopixel::setup() {
  this->parent_->setup_neopixel(this->pin_);
}

light::LightTraits SeesawNeopixel::get_traits() {
  auto traits = light::LightTraits();
  traits.set_supported_color_modes({light::ColorMode::RGB});
  return traits;
}
void SeesawNeopixel::write_state(light::LightState *state) {
  float red, green, blue;
  state->current_values_as_rgb(&red, &green, &blue);
  this->parent_->color_neopixel(red * 255, green * 255, blue * 255);
}

}  // namespace seesaw
}  // namespace esphome

