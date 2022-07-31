#include "sm2235.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace sm2235 {

static const char *const TAG = "sm2235";

void SM2235::setup() {
  this->sda_pin_->setup();
  this->scl_pin_->setup();
  this->scl_pin_->digital_write(true);
  this->sda_pin_->digital_write(true);
}

void SM2235::dump_config() {
  ESP_LOGCONFIG(TAG, "SM2235:");
  ESP_LOGCONFIG(TAG, "  Channels: %d", this->num_channels_);
}

void SM2235::start_() {
  this->sda_pin_->digital_write(false);
  delayMicroseconds(4);
}

void SM2235::stop_() {
  this->sda_pin_->digital_write(true);
  delayMicroseconds(2);
}

void SM2235::send_byte_(uint8_t value) {
  for (int i = 0; i < 9; i++, value <<= 1) {
    this->scl_pin_->digital_write(false);
    this->sda_pin_->digital_write(value & 0x80 > 0);
    delayMicroseconds(2);
    this->scl_pin_->digital_write(true);
    delayMicroseconds(2);
  }
}

void SM2235::update_(uint8_t channel) {
  uint8_t byte0 = 0xc0 + channel;
  switch(this->num_channels_) {
   case 0:
    break;
   case 1:
   case 2:
    byte0 |= 0x20; break;
   case 3:
    byte0 |= 0x10; break;
   case 4:
   case 5:
    byte0 |= 0x30; break;
  }
  this->start_();
  this->send_byte_(byte0);
  this->send_byte_(this->currents_);
  uint16_t value = this->values_[channel]; 
  this->send_byte_(value >> 8);
  this->send_byte_(value & 0xff);
  this->stop_();
}

void SM2235::register_channel(SM2235Channel *channel, uint8_t num) {
  this->num_channels_ = std::max(this->num_channels_, (uint8_t)(num + 1));
  channel->set_parent(this);
}

void SM2235::set_channel_value(uint8_t channel, uint16_t value) {
  this->values_[channel] = value;
  this->update_(channel);
}

void SM2235Channel::write_state(float state) {
  const uint16_t value = roundf(state * 1023);
  this->parent_->set_channel_value(this->channel_, value);
}

}  // namespace sm2235
}  // namespace esphome
