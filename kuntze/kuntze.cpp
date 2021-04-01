#include "kuntze.h"
#include "esphome/core/log.h"

namespace esphome {
namespace kuntze {

static const char *TAG = "kuntze";

static const uint8_t CMD_READ_REG = 0x03;
static const uint16_t REGISTER[] = {4136, 4160, 4680, 6000, 4688, 4728, 5832};

void Kuntze::on_modbus_data(const std::vector<uint8_t> &data) {
  auto get_16bit = [&](int i) -> uint16_t {
    return (uint16_t(data[i * 2]) << 8) | uint16_t(data[i * 2 + 1]);
  };

  waiting_ = false;
  if (data.size() != 4) {
    ESP_LOGW(TAG, "Invalid data packet size (%d) for state %d", data.size(), state_);
    return;
  }
  ESP_LOGVV(TAG, "Data: %s", hexencode(data).c_str());

  float value = (float)get_16bit(1);
  for (int i = 0; i < data[5]; i++)
    value /= 10.0;
  ESP_LOGD(TAG, "%d decimals", data[5]);
  switch (state_) {
  case 1:
    ESP_LOGD(TAG, "pH=%.1f", value);
    if (ph_sensor_ != nullptr)
      ph_sensor_->publish_state(value);
    break;
  case 2:
    ESP_LOGD(TAG, "temperature=%.1f", value);
    if (temperature_sensor_ != nullptr)
      temperature_sensor_->publish_state(value);
    break;
  case 3:
    ESP_LOGD(TAG, "DIS1=%.1f", value);
    if (dis1_sensor_ != nullptr)
      dis1_sensor_->publish_state(value);
    break;
  case 4:
    ESP_LOGD(TAG, "DIS2=%.1f", value);
    if (dis2_sensor_ != nullptr)
      dis2_sensor_->publish_state(value);
    break;
  case 5:
    ESP_LOGD(TAG, "REDOX=%.1f", value);
    if (redox_sensor_ != nullptr)
      redox_sensor_->publish_state(value);
    break;
  case 6:
    ESP_LOGD(TAG, "EC=%.1f", value);
    if (ec_sensor_ != nullptr)
      ec_sensor_->publish_state(value);
    break;
  case 7:
    ESP_LOGD(TAG, "OCI=%.1f", value);
    if (oci_sensor_ != nullptr)
      oci_sensor_->publish_state(value);
    break;
  }
  if (++state_ > 7)
    state_ = 0;
}

void Kuntze::loop() {
  long now = millis();
  // timeout after 15 seconds
  if (waiting_ && (now - last_send_ > 15000)) {
    ESP_LOGW(TAG, "timed out waiting for response");
    waiting_ = false;
  }
  if (waiting_ || (state_ == 0))
    return;
  last_send_ = now;
  send(CMD_READ_REG, REGISTER[state_ - 1], 2);
  waiting_ = true;
}

void Kuntze::update() { state_ = 1; }

void Kuntze::dump_config() {
  ESP_LOGCONFIG(TAG, "Kuntze:");
  ESP_LOGCONFIG(TAG, "  Address: 0x%02X", address_);
  LOG_SENSOR("", "pH", ph_sensor_);
  LOG_SENSOR("", "temperature", temperature_sensor_);
  LOG_SENSOR("", "DIS1", dis1_sensor_);
  LOG_SENSOR("", "DIS2", dis2_sensor_);
  LOG_SENSOR("", "REDOX", redox_sensor_);
  LOG_SENSOR("", "EC", ec_sensor_);
  LOG_SENSOR("", "OCI", oci_sensor_);
}

}  // namespace kuntze
}  // namespace esphome
