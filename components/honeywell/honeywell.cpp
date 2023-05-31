#include "honeywell.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace honeywell {

static const char *TAG = "honeywell";

void HoneyWell::loop() {
  while (this->available()) {
    uint8_t c;
    this->read_byte(&c);
    if (c != '\r') {
      if (rx_message_.size() < 30)
        this->rx_message_.push_back(c);
      continue;
    }
    std::string s(this->rx_message_.begin(), this->rx_message_.end());
    this->rx_message_.clear();
    ESP_LOGD(TAG, "received message: %s", s.c_str());
    // KEYS_XX[02] 0x01 0x0a
    if ((s.length() < 11) || (s.substr(0, 5) != "KEYS_")) {
      ESP_LOGD(TAG, "unknown message");
      continue;
    }
    int count = s[9] - '0';
    for (int i = 0; i < count; i++) {
      if (s.length() >= 11 + (i + 1) * 5) {
        uint8_t key;
        parse_hex(s.c_str() + 14 + i * 5, &key, 2);
        if (key <= 9)
          key += '0';
        else if (key == 0xa)
          key = '*';
        else if (key == 0xb)
          key = '#';
        else if ((key >= 0x1c) && (key <= 0x1f))
          key += 'A' - 0x1c;
        else
          continue;
        this->send_key_(key);
      }
    }
  }
}

void HoneyWell::dump_config() { ESP_LOGCONFIG(TAG, "Honeywell keypad:"); }

}  // namespace honeywell
}  // namespace esphome
