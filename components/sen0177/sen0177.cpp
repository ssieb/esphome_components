#include "sen0177.h"
#include "esphome/core/log.h"

namespace esphome {
namespace sen0177 {

static const char *TAG = "sen0177";

void SEN0177Component::dump_config() {
  ESP_LOGCONFIG(TAG, "SEN0177:");
  LOG_SENSOR("  ", "PM1.0", this->pm_1_0_sensor_);
  LOG_SENSOR("  ", "PM2.5", this->pm_2_5_sensor_);
  LOG_SENSOR("  ", "PM10.0", this->pm_10_0_sensor_);
  LOG_SENSOR("  ", "PMC0.3", this->pmc_0_3_sensor_);
  LOG_SENSOR("  ", "PMC0.5", this->pmc_0_5_sensor_);
  LOG_SENSOR("  ", "PMC1.0", this->pmc_1_0_sensor_);
  LOG_SENSOR("  ", "PMC2.5", this->pmc_2_5_sensor_);
  LOG_SENSOR("  ", "PMC5.0", this->pmc_5_0_sensor_);
  LOG_SENSOR("  ", "PMC10.0", this->pmc_10_0_sensor_);
  this->check_uart_settings(9600);
}

void SEN0177Component::loop() {
  const uint32_t now = millis();
  if ((now - this->last_transmission_ >= 500) && this->data_index_) {
    // last transmission too long ago. Reset RX index.
    ESP_LOGW(TAG, "Last transmission too long ago. Reset RX index.");
    this->data_index_ = 0;
  }

  if (!this->available())
    return;

  this->last_transmission_ = now;
  while (this->available()) {
    uint8_t c;
    this->read_byte(&c);
    if ((this->data_index_ == 0) && (c != 0x42))
      continue;
    this->data_[this->data_index_++] = c;
    if (this->data_index_ < 32)
      continue;
    this->data_index_ = 0;
    if (!this->check_data_()) {
      ESP_LOGW(TAG, "data check failed");
      continue;
    }
    this->parse_data_();
  }
}

bool SEN0177Component::check_data_() const {
  const uint8_t *data = this->data_;
  if (data[1] != 0x4d) // check second start byte
    return false;
  if (this->get_16_bit_uint_(2) != 28) // check packet length
    return false;
  // create checksum
  uint16_t sum = 0;
  for (uint8_t i = 0; i < 30; i++)
    sum += data[i];
  return (sum == this->get_16_bit_uint_(30));
}

void SEN0177Component::parse_data_() {
  const uint16_t pm_1_0 = this->get_16_bit_uint_(4);
  const uint16_t pm_2_5 = this->get_16_bit_uint_(6);
  const uint16_t pm_10_0 = this->get_16_bit_uint_(8);
  const uint16_t pmc_0_3 = this->get_16_bit_uint_(16);
  const uint16_t pmc_0_5 = this->get_16_bit_uint_(18);
  const uint16_t pmc_1_0 = this->get_16_bit_uint_(20);
  const uint16_t pmc_2_5 = this->get_16_bit_uint_(22);
  const uint16_t pmc_5_0 = this->get_16_bit_uint_(24);
  const uint16_t pmc_10_0 = this->get_16_bit_uint_(26);

  ESP_LOGD(TAG, "Got PM1.0: %d µg/m³, PM2.5: %d µg/m³, PM10.0: %d µg/m³",
           pm_1_0, pm_2_5, pm_10_0);
  ESP_LOGD(TAG, "Got PMC0.3: %d, PMC0.5: %d, PMC1.0: %d, PMC2.5: %d, PMC5.0: %d, PMC10.0: %d",
           pmc_0_3, pmc_0_5, pmc_1_0, pmc_2_5, pmc_5_0, pmc_10_0);

  if (this->pm_1_0_sensor_ != nullptr)
    this->pm_1_0_sensor_->publish_state(pm_1_0);
  if (this->pm_2_5_sensor_ != nullptr)
    this->pm_2_5_sensor_->publish_state(pm_2_5);
  if (this->pm_10_0_sensor_ != nullptr)
    this->pm_10_0_sensor_->publish_state(pm_10_0);

  if (this->pmc_0_3_sensor_ != nullptr)
    this->pmc_0_3_sensor_->publish_state(pmc_0_3);
  if (this->pmc_0_5_sensor_ != nullptr)
    this->pmc_0_5_sensor_->publish_state(pmc_0_5);
  if (this->pmc_1_0_sensor_ != nullptr)
    this->pmc_1_0_sensor_->publish_state(pmc_1_0);
  if (this->pmc_2_5_sensor_ != nullptr)
    this->pmc_2_5_sensor_->publish_state(pmc_2_5);
  if (this->pmc_5_0_sensor_ != nullptr)
    this->pmc_5_0_sensor_->publish_state(pmc_5_0);
  if (this->pmc_10_0_sensor_ != nullptr)
    this->pmc_10_0_sensor_->publish_state(pmc_10_0);
}

uint16_t SEN0177Component::get_16_bit_uint_(uint8_t start_index) const {
  return (uint16_t(this->data_[start_index]) << 8) | uint16_t(this->data_[start_index + 1]);
}

}  // namespace sen0177
}  // namespace esphome
