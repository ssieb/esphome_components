#include "clima.h"
#include "esphome/core/log.h"

namespace esphome {
namespace clima {

static const char *TAG = "clima";

void Clima::loop() {
  while (this->available()) {
    uint8_t c;
    this->read_byte(&c);
    if (!this->reading_) {
      if (c == 2) {
        this->reading_ = true;
        this->buffer_.clear();
      }
      continue;
    }
    if (c != 3) {
      this->buffer_.push_back(c);
      if (this->buffer_.size() > 62) {
	ESP_LOGE(TAG, "didn't find ETX");
        this->reading_ = false;
      }
      continue;
    }
    this->reading_ = false;
    if (this->buffer_.size() != 62) {
      ESP_LOGE(TAG, "data packet has the wrong length");
      continue;
    }
    uint8_t *data = this->buffer_.data();
    uint8_t csum = 0;
    for (int i = 0; i < 60; i++)
      csum ^= *data++;
    data = this->buffer_.data();
    uint8_t csumv;
    parse_hex((char *)&data[60], 2, &csumv, 1);
    if (csum != csumv) {
      ESP_LOGE(TAG, "checksum failed");
      continue;
    }
    handle_data_(data);
  }
}

void Clima::handle_data_(uint8_t *data) {
  for (int i = 0; i < 62; i++)
    if (data[i] == ';')
      data[i] = 0;
  if (this->time_sensor_ != nullptr)
    this->time_sensor_->publish_state(std::string((char *)data));
  if (this->brightness_east_sensor_ != nullptr)
    this->brightness_east_sensor_->publish_state(parse_number<float>((char *)&data[18]).value_or(NAN) * 1000);
  if (this->brightness_west_sensor_ != nullptr)
    this->brightness_west_sensor_->publish_state(parse_number<float>((char *)&data[30]).value_or(NAN) * 1000);
  if (this->brightness_south_sensor_ != nullptr)
    this->brightness_south_sensor_->publish_state(parse_number<float>((char *)&data[24]).value_or(NAN) * 1000);
  if (this->twilight_sensor_ != nullptr)
    this->twilight_sensor_->publish_state(parse_number<float>((char *)&data[36]).value_or(NAN));
  if (this->humidity_sensor_ != nullptr)
    this->humidity_sensor_->publish_state(parse_number<float>((char *)&data[40]).value_or(NAN));
  if (this->temperature_sensor_ != nullptr)
    this->temperature_sensor_->publish_state(parse_number<float>((char *)&data[44]).value_or(NAN));
  if (this->precipitation_bsensor_ != nullptr)
    this->precipitation_bsensor_->publish_state(data[50] == '1');
  if (this->windspeed_sensor_ != nullptr)
    this->windspeed_sensor_->publish_state(parse_number<float>((char *)&data[52]).value_or(NAN));
  uint8_t status;
  parse_hex((char *)&data[57], 2, &status, 1);
  if (this->low_voltage_bsensor_ != nullptr)
    this->low_voltage_bsensor_->publish_state(status & 1);
  if (this->no_time_sync_bsensor_ != nullptr)
    this->no_time_sync_bsensor_->publish_state(status & 2);
  if (this->heating_bsensor_ != nullptr)
    this->heating_bsensor_->publish_state(status & 4);
  if (this->time_syncing_bsensor_ != nullptr)
    this->time_syncing_bsensor_->publish_state(status & 8);
}

void Clima::dump_config() {
  //LOG_TEXT_SENSOR("", "The Text Sensor", this->the_text_);
  //LOG_SENSOR("", "The Sensor", this->the_sensor_);
}

}  // namespace clima
}  // namespace esphome
