#include "ht16k33_display.h"
#include "font.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace lcd_ht16k33 {

static const char *TAG = "lcd_ht16k33";

// First set bit determines command, bits after that are the data.
static const uint8_t LCD_DISPLAY_COMMAND_SET_DDRAM_ADDR = 0x00;
static const uint8_t LCD_DISPLAY_COMMAND_SYSTEM_SETUP = 0x21;
static const uint8_t LCD_DISPLAY_COMMAND_DISPLAY_OFF = 0x80;
static const uint8_t LCD_DISPLAY_COMMAND_DISPLAY_ON = 0x81;
static const uint8_t LCD_DISPLAY_COMMAND_DIMMING = 0xE0;

void HT16K33LCDDisplay::setup() {
  this->write_bytes(LCD_DISPLAY_COMMAND_SYSTEM_SETUP, nullptr, 0);
  this->write_bytes(LCD_DISPLAY_COMMAND_DISPLAY_ON, nullptr, 0);
  this->set_brightness(1);
}

float HT16K33LCDDisplay::get_setup_priority() const { return setup_priority::PROCESSOR; }

void HT16K33LCDDisplay::display() {
  this->write_bytes(LCD_DISPLAY_COMMAND_SET_DDRAM_ADDR, this->buffer_, 8);
}

void HT16K33LCDDisplay::update() {
  memset(this->buffer_, 0, 8);
  for (uint8_t i = 0; i < 8; i++)
    this->buffer_[i] = 0;

  this->call_writer();
  this->display();
}

void HT16K33LCDDisplay::set_brightness(float level) {
  int val = (int) round(level * 16);
  if (val < 0)
    val = 0;
  if (val > 16)
    val = 16;
  this->brightness_ = val;
  if (val == 0) {
    this->write_bytes(LCD_DISPLAY_COMMAND_DISPLAY_OFF, nullptr, 0);
  } else {
    this->write_bytes(LCD_DISPLAY_COMMAND_DIMMING + (val - 1), nullptr, 0);
    this->write_bytes(LCD_DISPLAY_COMMAND_DISPLAY_ON, nullptr, 0);
  }
}

float HT16K33LCDDisplay::get_brightness() {
  return this->brightness_ / 16.0;
}

void HT16K33LCDDisplay::print(const char *str) {
  uint8_t pos = 0;
  uint16_t fontc = 0;
  while (*str != '\0') {
    if (pos >= 8) {
      ESP_LOGW(TAG, "writing off the screen!");
      break;
    }

    uint8_t c = *reinterpret_cast<const uint8_t *>(str++);
    if (c > 127)
      fontc = 0;
    else
      fontc = pgm_read_word(&alphafonttable[c]);
    c = *reinterpret_cast<const uint8_t *>(str);
    if (c == '.') {
      fontc |= 0x8000;
      str++;
    }
    this->buffer_[pos++] = fontc >> 8;
    this->buffer_[pos++] = fontc & 0xff;
  }
}

void HT16K33LCDDisplay::print(const std::string &str) { this->print(str.c_str()); }

void HT16K33LCDDisplay::printf(const char *format, ...) {
  va_list arg;
  va_start(arg, format);
  char buffer[4];
  int ret = vsnprintf(buffer, sizeof(buffer), format, arg);
  va_end(arg);
  if (ret > 0)
    this->print(buffer);
}

#ifdef USE_TIME
void HT16K33LCDDisplay::strftime(const char *format, time::ESPTime time) {
  char buffer[4];
  size_t ret = time.strftime(buffer, sizeof(buffer), format);
  if (ret > 0)
    this->print(buffer);
}
#endif

}  // namespace lcd_ht16k33
}  // namespace esphome

