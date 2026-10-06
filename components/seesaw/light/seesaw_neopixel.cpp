#include "seesaw_neopixel.h"
#include "esphome/core/log.h"

namespace esphome {
namespace seesaw {

static const char *const TAG = "seesaw.neopixel";

void SeesawNeopixel::setup() {
  RAMAllocator<uint8_t> allocator;
  this->buf_ = allocator.allocate(this->num_leds_ * 3);
  if (this->buf_ == nullptr) {
    ESP_LOGE(TAG, "Failed to allocate buffer of size %u", this->num_leds_ * 3);
    this->mark_failed();
    return;
  }
  memset(this->buf_, 0x00, this->num_leds_ * 3);

  this->effect_data_ = allocator.allocate(this->num_leds_);
  if (this->effect_data_ == nullptr) {
    ESP_LOGE(TAG, "Failed to allocate effect data of size %u", this->num_leds_);
    this->mark_failed();
    return;
  }
  memset(this->effect_data_, 0x00, this->num_leds_);

  this->parent_->setup_neopixel(this->pin_, this->num_leds_);
}

void SeesawNeopixel::write_state(light::LightState *state) {
  this->parent_->write_neopixel_buffer(this->buf_, this->num_leds_ * 3);
  this->parent_->show_neopixel();
}

light::ESPColorView SeesawNeopixel::get_view_internal(int32_t index) const {
  size_t pos = index * 3;
  // red, green, blue, white, effect_data, color_correction -- buffer is laid out G, R, B per pixel.
  return {this->buf_ + pos + 1, this->buf_ + pos,       this->buf_ + pos + 2,
          nullptr,              this->effect_data_ + index, &this->correction_};
}

}  // namespace seesaw
}  // namespace esphome
