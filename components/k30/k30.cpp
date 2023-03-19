#include "k30.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace k30 {

static const char *const TAG = "k30";

static const uint8_t TMP102_ADDRESS = 0x48;
static const uint8_t TMP102_REGISTER_TEMPERATURE = 0x00;
static const uint8_t TMP102_REGISTER_CONFIGURATION = 0x01;
static const uint8_t TMP102_REGISTER_LOW_LIMIT = 0x02;
static const uint8_t TMP102_REGISTER_HIGH_LIMIT = 0x03;

static const float TMP102_CONVERSION_FACTOR = 0.0625;

static const uint8_t K30_READ[] = {0x22, 0, 8, 0x2a};

void K30Component::setup() { ESP_LOGCONFIG(TAG, "Setting up K30..."); }

void K30Component::dump_config() {
  ESP_LOGCONFIG(TAG, "K30:");
  LOG_I2C_DEVICE(this);
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Communication with K30 failed!");
  }
  LOG_UPDATE_INTERVAL(this);
  LOG_SENSOR("  ", "CO2", this);
}

void K30Component::loop() {
  if (!this->updating_ || (millis() - this->last_send_ < 20))
    return;
  this->updating_ = false;
  uint8_t buf[4];
  if (this->read(buf, 4) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "error reading from sensor");
    this->status_set_warning();
    return;
  }
  if (!(buf[0] & 1)) {
    ESP_LOGW(TAG, "transaction incomplete when reading from sensor");
    return;
  }
  if (buf[0] + buf[1] + buf[2] != buf[3]) {
    ESP_LOGW(TAG, "incorrect checksum when reading from sensor");
    return;
  }
  int16_t value = (buf[1] << 8) + buf[2];
  this->publish_state(value);
  this->status_clear_warning();
}

void K30Component::update() {
  if (this->write(K30_READ, 4) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "error reading from sensor");
    this->status_set_warning();
    return;
  }
  this->updating_ = true;
  this->last_send_ = millis();
}

float K30Component::get_setup_priority() const { return setup_priority::DATA; }

}  // namespace k30
}  // namespace esphome
