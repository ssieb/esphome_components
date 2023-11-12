#include "esphome/core/log.h"
#include "esphome/core/hal.h"
#include "ht16k33_alpha.h"
#include "font.h"

#ifndef USE_ESP8266
  #define pgm_read_word(s) (*s)
#endif

namespace esphome {
namespace ht16k33 {

static const char *TAG = "ht16k33";

void HT16K33AlphaDisplay::display_() {
  int offset = this->offset_;
  for (auto *display : this->displays_) {
    display->write_bytes(DISPLAY_COMMAND_SET_DDRAM_ADDR, this->buffer_ + offset, 8);
    offset += 8;
  }
}

uint16_t HT16K33AlphaDisplay::read_character_(uint8_t c) const {
  return pgm_read_word(&alphafonttable[c]);
}

}  // namespace ht16k33
}  // namespace esphome

