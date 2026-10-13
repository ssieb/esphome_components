#include "esphome/core/log.h"
#include "esphome/core/hal.h"
#include "ht16k33_7segment.h"
#include "font.h"

#ifndef USE_ESP8266
  #define pgm_read_word(s) (*s)
#endif

namespace esphome {
namespace ht16k33 {

static const char *TAG = "ht16k33";

void HT16K337SegmentDisplay::display_() {
  constexpr uint8_t size = 10;
  uint8_t buffer[size];
  uint8_t src_idx = this->offset_;
  for (auto *display : this->displays_) {
    for (uint8_t dst_idx = 0; dst_idx < size; dst_idx++) {
      if (dst_idx == 4) {
         buffer[dst_idx++] = this->show_colon_ ? 0x02 : 0;
         buffer[dst_idx] = 0;
      } else {
         buffer[dst_idx] = this->buffer_[src_idx++];
      }
    }
    display->write_bytes(DISPLAY_COMMAND_SET_DDRAM_ADDR, buffer, size);
  }
}

//void HT16K337SegmentDisplay::display_() {
//  int offset = this->offset_;
//  static const uint8_t size = this->display_size_();
//  uint8_t buffer[size];
//  memcpy(buffer, this->buffer_ + offset, 4);
//  offset += 4;
//  if (this->show_colon_) {
//    buffer[4] = 0x02;
//  } else {
//    buffer[4] = 0;
//  }
//  buffer[5] = 0;
//  memcpy(buffer + 6, this->buffer_ + offset, 4);
//  offset += 4;
//
//  for (auto *display : this->displays_) {
//    display->write_bytes(DISPLAY_COMMAND_SET_DDRAM_ADDR, buffer, size);
//    offset += 8;
//  }
//}

uint16_t HT16K337SegmentDisplay::read_character_(uint8_t c) const {
  return pgm_read_word(&sevensegfonttable[c - 32]);
}

}  // namespace ht16k33
}  // namespace esphome

