#include "seesaw.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace seesaw {

static const char *const TAG = "seesaw";

#define SEESAW_HW_ID_SAMD09 0x55
#define SEESAW_HW_ID_TINY806 0x84
#define SEESAW_HW_ID_TINY807 0x85
#define SEESAW_HW_ID_TINY816 0x86
#define SEESAW_HW_ID_TINY817 0x87
#define SEESAW_HW_ID_TINY1616 0x88
#define SEESAW_HW_ID_TINY1617 0x89

float Seesaw::get_setup_priority() const { return setup_priority::IO; }

static const char *cpuid_to_string(uint8_t id) {
  switch (id) {
   case SEESAW_HW_ID_SAMD09: return "SAMD09";
   case SEESAW_HW_ID_TINY806: return "ATtiny806";
   case SEESAW_HW_ID_TINY807: return "ATtiny807";
   case SEESAW_HW_ID_TINY816: return "ATtiny816";
   case SEESAW_HW_ID_TINY817: return "ATtiny817";
   case SEESAW_HW_ID_TINY1616: return "ATtiny1616";
   case SEESAW_HW_ID_TINY1617: return "ATtiny1617";
   default: return nullptr;
  }
}

void Seesaw::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Seesaw...");
  /*
  if (this->write8(SEESAW_STATUS, SEESAW_STATUS_SWRST, 0xff) != i2c::ERROR_OK) {
    this->mark_failed();
    return;
  }
  */
  uint8_t c = 0;
  this->readbuf(SEESAW_STATUS, SEESAW_STATUS_HW_ID, &c, 1);
  this->cpuid_ = c;
  uint8_t buf[4];
  this->readbuf(SEESAW_STATUS, SEESAW_STATUS_VERSION, buf, 4);
  this->version_ = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
  this->readbuf(SEESAW_STATUS, SEESAW_STATUS_OPTIONS, buf, 4);
  this->options_ = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
}

void Seesaw::loop() {
}

void Seesaw::dump_config() {
  ESP_LOGCONFIG(TAG, "Seesaw module:");
  LOG_I2C_DEVICE(this);
  const char *cpu = cpuid_to_string(this->cpuid_);
  if (cpu != nullptr) {
    ESP_LOGCONFIG(TAG, "  CPU: %s", cpu);
  } else {
    ESP_LOGCONFIG(TAG, "  CPU: unknown (%02x)", this->cpuid_);
  }
  uint32_t v = this->version_;
  ESP_LOGCONFIG(TAG, "  Version: %d-%02d-%02d %u", v & 0x3f, (v >> 7) & 0xf, (v >> 11) & 0x1f, v >> 16);
  ESP_LOGCONFIG(TAG, "  Options: %08x", this->options_);
  /*
  if (this->options_ & (1 << SEESAW_GPIO))
    ESP_LOGCONFIG(TAG, "    GPIO");
  if (this->options_ & (1 << SEESAW_SERCOM0))
    ESP_LOGCONFIG(TAG, "    Serial");
  if (this->options_ & (1 << SEESAW_TIMER))
    ESP_LOGCONFIG(TAG, "    Timer");
  if (this->options_ & (1 << SEESAW_ADC))
    ESP_LOGCONFIG(TAG, "    ADC");
  if (this->options_ & (1 << SEESAW_DAC))
    ESP_LOGCONFIG(TAG, "    DAC");
  if (this->options_ & (1 << SEESAW_INTERRUPT))
    ESP_LOGCONFIG(TAG, "    Interrupt");
  if (this->options_ & (1 << SEESAW_DAP))
    ESP_LOGCONFIG(TAG, "    DAP");
  if (this->options_ & (1 << SEESAW_EEPROM))
    ESP_LOGCONFIG(TAG, "    EEPROM");
  if (this->options_ & (1 << SEESAW_NEOPIXEL))
    ESP_LOGCONFIG(TAG, "    NeoPixel");
  if (this->options_ & (1 << SEESAW_TOUCH))
    ESP_LOGCONFIG(TAG, "    Touch");
  if (this->options_ & (1 << SEESAW_KEYPAD))
    ESP_LOGCONFIG(TAG, "    Keypad");
  if (this->options_ & (1 << SEESAW_ENCODER))
    ESP_LOGCONFIG(TAG, "    Encoder");
  */
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

int16_t Seesaw::get_touch_value(uint8_t channel) {
  uint8_t buf[2];
  if (this->readbuf(SEESAW_TOUCH, SEESAW_TOUCH_CHANNEL_OFFSET + channel, buf, 2) != i2c::ERROR_OK)
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

uint16_t Seesaw::analog_read(uint8_t pin) {
  uint8_t buf[2];
  this->readbuf(SEESAW_ADC, SEESAW_ADC_CHANNEL_OFFSET + pin, buf, 2);
  return (buf[0] << 8) + buf[1];
}

bool Seesaw::digital_read(uint8_t pin) {
  uint32_t pins = 1 << pin;
  uint8_t buf[4];
  this->readbuf(SEESAW_GPIO, SEESAW_GPIO_BULK, buf, 4);
  uint32_t ret = (buf[0] << 24) + (buf[1] << 16) + (buf[2] << 8) + buf[3];
  return ret & pins;
}

void Seesaw::digital_write(uint8_t pin, bool state) {
  uint32_t pins = 1 << pin;
  if (state)
    this->write32(SEESAW_GPIO, SEESAW_GPIO_BULK_SET, pin);
  else
    this->write32(SEESAW_GPIO, SEESAW_GPIO_BULK_CLR, pin);
}

void Seesaw::setup_neopixel(int pin, uint16_t n) {
  this->write8(SEESAW_NEOPIXEL, SEESAW_NEOPIXEL_SPEED, 1);
  this->write16(SEESAW_NEOPIXEL, SEESAW_NEOPIXEL_BUF_LENGTH, n * 3);
  this->write8(SEESAW_NEOPIXEL, SEESAW_NEOPIXEL_PIN, pin);
}

void Seesaw::color_neopixel(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
  uint16_t offset = n * 3;
  uint8_t buf[7] = {SEESAW_NEOPIXEL, SEESAW_NEOPIXEL_BUF, (offset >> 8), offset, g, r, b};
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

void SeesawGPIOPin::setup() { pin_mode(flags_); }

void SeesawGPIOPin::pin_mode(gpio::Flags flags) { this->parent_->set_pinmode(this->pin_, flags); }

bool SeesawGPIOPin::digital_read() {
  return this->parent_->digital_read(this->pin_) != this->inverted_;
}

void SeesawGPIOPin::digital_write(bool value) {
  this->parent_->digital_write(this->pin_, value != this->inverted_);
}

std::string SeesawGPIOPin::dump_summary() const {
  return str_sprintf("%u via SeeSaw", this->pin_);
}

}  // namespace seesaw
}  // namespace esphome
