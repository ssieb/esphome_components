#include "mcp342x.h"
#include "esphome/core/log.h"

namespace esphome {
namespace mcp342x {

static const char *TAG = "mcp342x";

void MCP342XComponent::setup() {
  ESP_LOGD(TAG, "Setting up MCP342X...");
  uint8_t data[4];
  if (!this->read_bytes_raw(data, 4)) {
    this->mark_failed();
    return;
  }
}

void MCP342XComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Setting up MCP342X...");
  LOG_I2C_DEVICE(this);
  if (this->is_failed())
    ESP_LOGE(TAG, "Communication with MCP342X failed!");

  for (auto *sensor : this->sensors_) {
    LOG_SENSOR("  ", "Sensor", sensor);
    ESP_LOGCONFIG(TAG, "    Channel: %u", sensor->get_channel());
    ESP_LOGCONFIG(TAG, "    Gain: %u", 1 << sensor->get_gain());
    ESP_LOGCONFIG(TAG, "    Bits: %u", 12 + 2 * sensor->get_resolution());
  }
}

void MCP342XComponent::request_measurement(MCP342XSensor *sensor) {
  this->queue_.insert(this->queue_.begin(), sensor);
  return;
}

void MCP342XComponent::loop() {
  uint8_t data[4];
  MCP342XSensor *sensor;

  if (!this->busy_) {
    if (this->queue_.empty())
      return;

    sensor = this->queue_.back();
    uint8_t config = 0x80;
    config |= (sensor->get_channel() & 0x3) << 5;
    config |= (sensor->get_resolution() & 0x3) << 2;
    config |= (sensor->get_gain() & 0x3);

    if (!this->write_bytes_raw(&config, 1)) {
      this->status_set_warning();
      this->queue_.pop_back();
      sensor->update_result(NAN);
    }
    this->busy_ = true;
    return;
  }

  if (!this->read_bytes_raw(data, 4)) {
    this->mark_failed();
    this->status_set_warning();
    sensor = this->queue_.back();
    this->queue_.pop_back();
    sensor->update_result(NAN);
    this->busy_ = false;
    return;
  }
  uint8_t status = data[3];
  if ((status & 0xc) != 0xc) {
    status = data[2];
    data[2] = 0;
  }
  if (status & 0x80) // still converting
    return;
  int gain = 1 << (status & 3);
  int bits = 12 + ((status & 0xc) >> 1);
  long raw = (data[0] << 16) | (data[1] << 8) | data[2];
  if (data[0] & 0x80)
    raw |= 0xff000000;
  switch (bits) {
    case 12:
      raw <<= 4;
      break;
    case 14:
      raw <<= 2;
      break;
    case 16:
      break;
    case 18:
      raw <<= 6;
      break;
  }
  float value = 2.048 * (float(raw) / 0x01000000) / gain;
  sensor = this->queue_.back();
  this->queue_.pop_back();
  sensor->update_result(value);
  this->busy_ = false;
}

void MCP342XSensor::update() {
  if (this->busy_)
    return;
  this->busy_ = true;
  this->parent_->request_measurement(this);
}

void MCP342XSensor::update_result(float value) {
  this->busy_ = false;
  if (!isnan(value)) {
    ESP_LOGD(TAG, "'%s': Got Voltage=%fV", this->get_name().c_str(), value);
    this->publish_state(value);
  }
}

}  // namespace mcp342x
}  // namespace esphome
