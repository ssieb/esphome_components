#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include "esphome/core/hal.h"
#include "sn74hc595_display.h"

namespace esphome {
namespace sn74hc595 {

static const char *const TAG = "display.sn74hc595";

uint8_t digits[] = { 0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90 };

void SN74HC595Display::setup() {
  ESP_LOGCONFIG(TAG, "Setting up SN74HC595 display...");
}

void SN74HC595Display::loop() {
  static int pos = 0;
  this->parent_->set_output_bits((1 << pos) + (buffer_[pos] << 8));
  if (++pos > 3)
    pos = 0;
}

void SN74HC595Display::dump_config() {
  ESP_LOGCONFIG(TAG, "SN74HC595:");
  LOG_UPDATE_INTERVAL(this);
}

void SN74HC595Display::update() {
  for (uint8_t &i : this->buffer_)
    i = 0;
  if (this->writer_.has_value())
    (*this->writer_)(*this);
  this->display();
}

float SN74HC595Display::get_setup_priority() const { return setup_priority::PROCESSOR; }

void SN74HC595Display::display() {
  ESP_LOGVV(TAG, "Display %02X%02X%02X%02X", buffer_[0], buffer_[1], buffer_[2], buffer_[3]);
}

uint8_t SN74HC595Display::print(uint8_t start_pos, const char *str) {
  ESP_LOGV(TAG, "Print at %d: %s", start_pos, str);
  uint8_t pos = start_pos;
  for (; *str != '\0'; str++) {
    uint8_t data;
    if (*str >= '0' && *str <= '9') {
      data = digits[*str - '0'];
    } else if (*str == '.') {
      if (pos != start_pos) {
        pos--;
	data = this->buffer_[pos] & 0x7f;
      } else {
        data = 0x7f;
      }
    } else {
      ESP_LOGW(TAG, "Encountered character '%c' with no representation while translating string!", *str);
      continue;
    }
    if (pos >= 4) {
      ESP_LOGE(TAG, "String is too long for the display!");
      break;
    }
    this->buffer_[pos++] = data;
  }
  return pos - start_pos;
}

uint8_t SN74HC595Display::print(const char *str) { return this->print(0, str); }

uint8_t SN74HC595Display::printf(uint8_t pos, const char *format, ...) {
  va_list arg;
  va_start(arg, format);
  char buffer[64];
  int ret = vsnprintf(buffer, sizeof(buffer), format, arg);
  va_end(arg);
  if (ret > 0)
    return this->print(pos, buffer);
  return 0;
}

uint8_t SN74HC595Display::printf(const char *format, ...) {
  va_list arg;
  va_start(arg, format);
  char buffer[64];
  int ret = vsnprintf(buffer, sizeof(buffer), format, arg);
  va_end(arg);
  if (ret > 0)
    return this->print(buffer);
  return 0;
}

#ifdef USE_TIME
uint8_t SN74HC595Display::strftime(uint8_t pos, const char *format, time::ESPTime time) {
  char buffer[64];
  size_t ret = time.strftime(buffer, sizeof(buffer), format);
  if (ret > 0)
    return this->print(pos, buffer);
  return 0;
}
uint8_t TM1637Display::strftime(const char *format, time::ESPTime time) { return this->strftime(0, format, time); }
#endif

}  // namespace sn74hc595
}  // namespace esphome
