#include "reinhardt.h"
#include "esphome/core/log.h"

namespace esphome {
namespace reinhardt {

static const char *TAG = "reinhardt";

static const std::array<const char *, RS_MAX> SENSOR_TAGS = {
  "TE", "FE", "TD", "DR", "WR", "WV", "WG", "WS", "WD", "RE", "RD", "SO", "ZA", "ZB", "ZC", "ZD", "ZE", "ZF", "UV", "LX", "DB", "TP", "WC",
  "OH", "GH", "GX", "GY", "GV", "GS"};

static SensorType find_sensor_type(const char *s) {
  for (int i = 0; i < RS_MAX; i++) {
    const char *sname = SENSOR_TAGS[i];
    if (s[0] == sname[0] && s[1] == sname[1])
      return (SensorType)i;
  }
  return RS_MAX;
}

void Reinhardt::loop() {
  while (this->available()) {
    uint8_t c;
    this->read_byte(&c);
    if (c == '\r')
      continue;
    if (c == '\n')
      this->parse_values_();
    else
      this->rx_message_.push_back(c);
  }
}

void Reinhardt::parse_values_() {
  std::string s(this->rx_message_.begin(), this->rx_message_.end());
  int spos = 0;
  int epos = 0;
  std::vector<std::pair<SensorType, float>> values;
  while (epos != std::string::npos) {
    epos = s.find(',', spos);
    int len = (epos == std::string::npos ? s.size() : epos) - spos;
    if (len >= 3) {
      s[epos] = '\0';
      SensorType type = find_sensor_type(&s[spos]);
      float value = parse_number<float>(&s[spos + 2]).value_or(NAN);
      values.emplace_back(std::make_pair(type, value));
      ESP_LOGV(TAG, "data: '%s', type: %d, value: %f", &s[spos], type, value);
    }
    if (epos != std::string::npos)
      spos = epos + 1;
    if (s[spos] == ' ')
      spos++;
  }
  this->rx_message_.clear();
  for (auto value : values) {
    if (value.first == RS_MAX)
      continue;
    sensor::Sensor *sensor = this->sensors_[value.first];
    if (sensor != nullptr)
      sensor->publish_state(value.second);
  }
}

void Reinhardt::dump_config() {
  ESP_LOGCONFIG(TAG, "Reinhardt weather station:");
  for (auto sens : this->sensors_) {
    LOG_SENSOR(TAG, "", sens);
  }
}

}  // namespace reinhardt
}  // namespace esphome
