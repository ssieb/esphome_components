#include "gpstime.h"
#include "esphome/core/log.h"

namespace esphome {
namespace gpstime {

static const char *TAG = "GPS.time";

void GPSTime::loop() {
  while (this->available()) {
    uint8_t c;
    this->read_byte(&c);
    if (this->have_time_)
      continue;
    if (c == '$') {
      this->receiving_ = true;
      this->ending_ = false;
      this->rx_buffer_.clear();
      continue;
    }
    if (!this->receiving_)
      continue;
    if (c == '\r') {
      if (this->ending_) {
        ESP_LOGW(TAG, "unexpected return");
        this->receiving_ = false;
      } else {
        this->ending_ = true;
      }
      continue;
    }
    if (c == '\n') {
      this->receiving_ = false;
      if (!this->ending_) {
        ESP_LOGW(TAG, "unexpected newline");
        continue;
      }
      this->handle_message_();
      continue;
    }
    if (this->ending_) {
      ESP_LOGW(TAG, "unexpected character");
      this->receiving_ = false;
      continue;
    }
    if (this->rx_buffer_.size() < 90)
      this->rx_buffer_.push_back(c);
  }
}

void GPSTime::handle_message_() {
  std::vector<std::string> terms;
  std::string data(this->rx_buffer_.begin(), this->rx_buffer_.end());
  ESP_LOGD(TAG, "received: %s", data.c_str());
  int start = 0;
  int end;
  int csum_pos = data.find('*');
  if (csum_pos != std::string::npos) {
    if (data.size() - csum_pos != 3) {
      ESP_LOGE(TAG, "invalid checksum data");
      return;
    }
    uint8_t csum = 0;
    parse_hex(data.c_str() + csum_pos + 1, &csum, 1);
    data.resize(csum_pos);
    uint8_t ccsum = 0;
    for (auto c : data)
      ccsum ^= c;
    if (csum != ccsum) {
      ESP_LOGE(TAG, "checksum failed: %02x != %02x", csum, ccsum);
      return;
    }
  }
  do {
    end = data.find(',', start);
    int len = end != std::string::npos ? end - start : end;
    terms.push_back(data.substr(start, len));
    start = end + 1;
  } while (end != std::string::npos);
  ESP_LOGD(TAG, "message: %s", terms[0].c_str());
  if (terms[0] != "GPRMC")
    return;
  ESP_LOGD(TAG, "time: '%s', date: '%s'", terms[1].c_str(), terms[9].c_str());
  if ((terms[1].size() < 6) || (terms[9].size() < 6))
    return;
  uint32_t timeinfo = int(parse_number<float>(terms[1]).value());
  uint32_t dateinfo = int(parse_number<uint32_t>(terms[9]).value());
  ESP_LOGD(TAG, "processing %d %d", timeinfo, dateinfo);
  ESPTime now{};
  now.year = dateinfo % 100 + 2000;
  now.month = dateinfo / 100 % 100;
  now.day_of_month = dateinfo / 10000;
  // Set these to valid value for  recalc_timestamp_utc - it's not used for calculation
  now.day_of_week = 1;
  now.day_of_year = 1;

  now.hour = timeinfo / 10000;
  now.minute = timeinfo / 100 % 100;
  now.second = timeinfo % 100;
  now.recalc_timestamp_utc(false);
  this->synchronize_epoch_(now.timestamp);
  this->have_time_ = true;
}

void GPSTime::update() { this->have_time_ = false; }

void GPSTime::dump_config() { ESP_LOGCONFIG("", "GPS time", this); }

}  // namespace gpstime
}  // namespace esphome
