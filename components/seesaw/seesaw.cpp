#include "seesaw.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace seesaw {

static const char *const TAG = "seesaw";

#define SEESAW_HW_ID_SAMD09 0x55
#define SEESAW_HW_ID_TINY8X7 0x87

float Seesaw::get_setup_priority() const { return setup_priority::IO; }

void Seesaw::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Seesaw...");
  if (this->write8(SEESAW_STATUS, SEESAW_STATUS_SWRST, 0xff) != i2c::ERROR_OK) {
    this->mark_failed();
    return;
  }
  uint8_t c = 0;
  this->readbuf(SEESAW_STATUS, SEESAW_STATUS_HW_ID, &c, 1);
  std::string cpu;
  if (c == SEESAW_HW_ID_SAMD09)
    cpu = "SAMD09";
  else if (c == SEESAW_HW_ID_TINY8X7)
    cpu = "TINY8X7";
  else
    cpu = "unknown";
  ESP_LOGCONFIG(TAG, "Hardware type is %s", cpu.c_str());
}

void Seesaw::enable_encoder(uint8_t number) {
  this->write8(SEESAW_ENCODER, SEESAW_ENCODER_INTENSET + number, 0x01);
}

int32_t Seesaw::get_encoder_position(uint8_t number) {
  uint8_t buf[4];
  if (this->readbuf(SEESAW_ENCODER, SEESAW_ENCODER_POSITION + number, buf, 4) != i2c::ERROR_OK)
    return 0;
  int32_t value = (buf[0] << 24) + (buf[1] << 16) + (buf[2] << 8) + buf[3];
  return -value;  // make clockwise positive
}

int16_t Seesaw::get_touch_value(uint8_t pin) {
  uint8_t buf[2];
  if (this->readbuf(SEESAW_TOUCH, SEESAW_TOUCH_CHANNEL_OFFSET + pin, buf, 2) != i2c::ERROR_OK)
    return -1;
  return (buf[0] << 8) | buf[1];
}

float Seesaw::get_temperature() {
  uint8_t buf[4];
  if (this->readbuf(SEESAW_STATUS, SEESAW_STATUS_TEMP, buf, 4) != i2c::ERROR_OK)
    return 0;
  int32_t value = (buf[0] << 24) + (buf[1] << 16) + (buf[2] << 8) + buf[3];
  return float(value) / 0x10000;
}

void Seesaw::set_pinmode(uint8_t pin, uint8_t mode) {
  uint32_t pins = 1 << pin;
  switch (mode) {
    case gpio::FLAG_OUTPUT:
    this->write32(SEESAW_GPIO, SEESAW_GPIO_DIRSET_BULK, pins);
    break;
   case gpio::FLAG_INPUT:
    this->write32(SEESAW_GPIO, SEESAW_GPIO_DIRCLR_BULK, pins);
    break;
   case gpio::FLAG_PULLUP:
    this->write32(SEESAW_GPIO, SEESAW_GPIO_DIRCLR_BULK, pins);
    this->write32(SEESAW_GPIO, SEESAW_GPIO_PULLENSET, pins);
    this->write32(SEESAW_GPIO, SEESAW_GPIO_BULK_SET, pins);
    break;
   case gpio::FLAG_PULLDOWN:
    this->write32(SEESAW_GPIO, SEESAW_GPIO_DIRCLR_BULK, pins);
    this->write32(SEESAW_GPIO, SEESAW_GPIO_PULLENSET, pins);
    this->write32(SEESAW_GPIO, SEESAW_GPIO_BULK_CLR, pins);
    break;
  }
}

void Seesaw::set_gpio_interrupt(uint32_t pin, bool enabled) {
  uint32_t pins = 1 << pin;
  if (enabled)
    this->write32(SEESAW_GPIO, SEESAW_GPIO_INTENSET, pins);
  else
    this->write32(SEESAW_GPIO, SEESAW_GPIO_INTENCLR, pins);
}

bool Seesaw::digital_read(uint8_t pin) {
  uint32_t pins = 1 << pin;
  uint8_t buf[4];
  this->readbuf(SEESAW_GPIO, SEESAW_GPIO_BULK, buf, 4);
  uint32_t ret = (buf[0] << 24) + (buf[1] << 16) + (buf[2] << 8) + buf[3];
  return ret & pins;
}

void Seesaw::setup_neopixel() {
  this->write8(SEESAW_NEOPIXEL, SEESAW_NEOPIXEL_SPEED, 1);
  this->write16(SEESAW_NEOPIXEL, SEESAW_NEOPIXEL_BUF_LENGTH, 3);
  this->write8(SEESAW_NEOPIXEL, SEESAW_NEOPIXEL_PIN, 6);
}

void Seesaw::color_neopixel(uint8_t r, uint8_t g, uint8_t b) {
  uint8_t buf[7] = {SEESAW_NEOPIXEL, SEESAW_NEOPIXEL_BUF, 0, 0, g, r, b};
  this->write(buf, 7);
  buf[1] = SEESAW_NEOPIXEL_SHOW;
  this->write(buf, 2);
}

i2c::ErrorCode Seesaw::write8(SeesawModule mod, uint8_t reg, uint8_t value) {
  uint8_t buf[3] = {mod, reg, value};
  return this->write(buf, 3);
}

i2c::ErrorCode Seesaw::write16(SeesawModule mod, uint8_t reg, uint16_t value) {
  uint8_t buf[4] = {mod, reg, (uint8_t)(value >> 8), (uint8_t)value};
  return this->write(buf, 4);
}

i2c::ErrorCode Seesaw::write32(SeesawModule mod, uint8_t reg, uint32_t value) {
  uint8_t buf[6] = {mod, reg, (uint8_t)(value >> 24), (uint8_t)(value >> 16),
                    (uint8_t)(value >> 8), (uint8_t)value};
  return this->write(buf, 6);
}

i2c::ErrorCode Seesaw::readbuf(SeesawModule mod, uint8_t reg, uint8_t *buf, uint8_t len) {
  uint8_t sendbuf[2] = {mod, reg};
  i2c::ErrorCode err = this->write(sendbuf, 2);
  if (err != i2c::ERROR_OK)
    return err;
  return this->read(buf, len);
}

/*
void SeesawGPIOPin::setup() { pin_mode(flags_); }
void SeesawGPIOPin::pin_mode(gpio::Flags flags) { this->parent_->pin_mode(this->pin_, flags); }
bool SeesawGPIOPin::digital_read() { return this->parent_->digital_read(this->pin_) != this->inverted_; }
void SeesawGPIOPin::digital_write(bool value) { this->parent_->digital_write(this->pin_, value != this->inverted_); }
std::string SeesawGPIOPin::dump_summary() const {
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "%u via SeeSaw", pin_);
  return buffer;
}
*/

}  // namespace seesaw
}  // namespace esphome
