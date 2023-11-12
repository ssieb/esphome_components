#include "esphome/core/log.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"
#include "ht16k33_display.h"

namespace esphome {
namespace ht16k33 {

static const char *TAG = "ht16k33";

void HT16K33BaseDisplay::dump_config() {
  ESP_LOGCONFIG(TAG, "HT16K33 Display:");
  LOG_I2C_DEVICE(this);
}

void HT16K33BaseDisplay::setup() {
  for (auto *display : this->displays_) {
    display->write_bytes(DISPLAY_COMMAND_SYSTEM_SETUP, nullptr, 0);
    display->write_bytes(DISPLAY_COMMAND_DISPLAY_ON, nullptr, 0);
  }
  this->set_brightness(1);
  memset(this->buffer_, 0, 64);
}

void HT16K33BaseDisplay::loop() {
  unsigned long now = millis();
  int numc = this->displays_.size() * 8;
  // check if the buffer has shrunk past the current position since last update
  if (this->offset_ + numc > this->buffer_fill_) {
    this->offset_ = std::max(this->buffer_fill_ - numc, 0);
    this->display_();
  }
  if (!this->scroll_ || (this->buffer_fill_ <= numc))
    return;
  if ((this->offset_ == 0) && (now - this->last_scroll_ < this->scroll_delay_))
    return;
  if (this->offset_ + numc >= this->buffer_fill_) {
    if (now - this->last_scroll_ >= this->scroll_dwell_) {
      this->offset_ = 0;
      this->last_scroll_ = now;
      this->display_();
    }
  } else
  if (now - this->last_scroll_ >= this->scroll_speed_) {
    this->offset_ += 2;
    this->last_scroll_ = now;
    this->display_();
  }
}

float HT16K33BaseDisplay::get_setup_priority() const { return setup_priority::PROCESSOR; }

void HT16K33BaseDisplay::update() {
  memset(this->buffer_, 0, 64);
  int prev_fill = this->buffer_fill_;
  this->buffer_fill_ = 0;
  this->call_writer();
  if (this->scroll_ && (prev_fill != this->buffer_fill_)) {
    this->last_scroll_ = millis();
    this->offset_ = 0;
  }
  this->display_();
}

void HT16K33BaseDisplay::set_brightness(float level) {
  int val = (int) round(level * 16);
  if (val < 0)
    val = 0;
  if (val > 16)
    val = 16;
  this->brightness_ = val;
  for (auto *display : this->displays_) {
    if (val == 0) {
      display->write_bytes(DISPLAY_COMMAND_DISPLAY_OFF, nullptr, 0);
    } else {
      display->write_bytes(DISPLAY_COMMAND_DIMMING + (val - 1), nullptr, 0);
      display->write_bytes(DISPLAY_COMMAND_DISPLAY_ON, nullptr, 0);
    }
  }
}

float HT16K33BaseDisplay::get_brightness() {
  return this->brightness_ / 16.0;
}

void HT16K33BaseDisplay::print(const char *str) {
  uint8_t pos = this->buffer_fill_;
  uint8_t idx = 1;
  uint16_t fontc = 0;
  this->show_colon_ = false;
  while (*str != '\0') {
    if (pos >= 64) {
      ESP_LOGW(TAG, "output buffer full!");
      break;
    }

    uint8_t c = *reinterpret_cast<const uint8_t *>(str++);
    if (c > 127)
      fontc = 0;
    else
      fontc = read_character_(c);
    c = *reinterpret_cast<const uint8_t *>(str);
    if (c == '.') {
      fontc |= decimal_point_mask_();
      str++;
      idx++;
    }
    if (c == ':' && idx == 2 && this->supports_colon_()) {
      this->show_colon_ = true;
      str++;
      idx++;
    }
    this->buffer_[pos++] = fontc & 0xff;
    this->buffer_[pos++] = fontc >> 8;
    idx++;
  }
  this->buffer_fill_ = pos;
}

void HT16K33BaseDisplay::print(const std::string &str) { this->print(str.c_str()); }

void HT16K33BaseDisplay::printf(const char *format, ...) {
  va_list arg;
  va_start(arg, format);
  char buffer[64];
  int ret = vsnprintf(buffer, sizeof(buffer), format, arg);
  va_end(arg);
  if (ret > 0)
    this->print(buffer);
}

#ifdef USE_TIME
void HT16K33BaseDisplay::strftime(const char *format, ESPTime time) {
  char buffer[64];
  size_t ret = time.strftime(buffer, sizeof(buffer), format);
  if (ret > 0)
    this->print(buffer);
}
#endif

}  // namespace ht16k33
}  // namespace esphome

