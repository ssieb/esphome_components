#include "ip5306.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace ip5306 {

static const char *const TAG = "ip5306";

static const uint8_t IP5306_DEFAULT_SETUP = 0x37;  // set power boost keep on by default
static const uint8_t IP5306_REG_CHARGE_OUT_BIT = 0x10;  // charge out bit

static const uint8_t IP5306_REG_CHG_DIG = 0x24;  // charge current
static const uint8_t IP5306_REG_CHG_CTL0 = 0x20;  // charge voltage

static const uint8_t IP5306_REG_SYS_CTL0 = 0x00;  // initialize
static const uint8_t IP5306_REG_SYS_CTL1 = 0x01;  // sys control 1
static const uint8_t IP5306_REG_SYS_CTL2 = 0x02;  // sys control 2
static const uint8_t IP5306_REG_READ0 = 0x70;  // charge en
static const uint8_t IP5306_REG_READ1 = 0x71;  // charge full
static const uint8_t IP5306_REG_LEVEL = 0x78;  // bat level

static const uint8_t CURRENT_400MA = 0x01 << 2;
static const uint8_t BAT_4_2V = 0x00;

//- REG_CTL0
static const uint8_t BOOST_ENABLE_BIT = 0x20;
static const uint8_t CHARGE_OUT_BIT = 0x10;
static const uint8_t BOOT_ON_LOAD_BIT = 0x04;
static const uint8_t BOOST_OUT_BIT = 0x02;
static const uint8_t BOOST_BUTTON_EN_BIT = 0x01;

//- REG_CTL1
static const uint8_t BOOST_SET_BIT = 0x80;
static const uint8_t WLED_SET_BIT = 0x40;
static const uint8_t SHORT_BOOST_BIT = 0x20;
static const uint8_t VIN_ENABLE_BIT = 0x04;

//- REG_CTL2
static const uint8_t SHUTDOWNTIME_MASK = 0x0c;
static const uint8_t SHUTDOWNTIME_64S = 0x0c;
static const uint8_t SHUTDOWNTIME_32S = 0x04;
static const uint8_t SHUTDOWNTIME_16S = 0x08;
static const uint8_t SHUTDOWNTIME_8S = 0x00;

float IP5306::get_setup_priority() const { return setup_priority::IO; }

void IP5306::setup() {
  ESP_LOGD(TAG, "Setting up ip5306...");
  if (this->write_register(IP5306_REG_SYS_CTL0, &IP5306_DEFAULT_SETUP, 1) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "setup failed");
    this->mark_failed();
    return;
  }
  ESP_LOGI(TAG, "ip5306 initial setup done");

  if (!setVinMaxCurrent(CURRENT_400MA)) {
    ESP_LOGE(TAG, "setVinMaxCurrent failed");
    this->mark_failed();
    return;
  }
  ESP_LOGI(TAG, "ip5306 setVinMaxCurrent done");
  if (!setChargeVolt(BAT_4_2V)) {
    ESP_LOGE(TAG, "setChargeVolt failed");
    this->mark_failed();
    return;
  }
  ESP_LOGI(TAG, "ip5306 setChargeVolt done");

  if (!setPowerBoostOnOff(true)) {
    ESP_LOGE(TAG, "setPowerBoostOnOff failed");
    this->mark_failed();
    return;
  }
  ESP_LOGI(TAG, "ip5306 setPowerBoostOnOff done");

  if (!setPowerBoostSet(true)) {
    ESP_LOGE(TAG, "setPowerBoostSet failed");
    this->mark_failed();
    return;
  }
  ESP_LOGI(TAG, "ip5306 setPowerBoostSet done");

  if (!setPowerVin(true)) {
    ESP_LOGE(TAG, "setPowerVin failed");
    this->mark_failed();
    return;
  }
  ESP_LOGI(TAG, "ip5306 setPowerVin done");

  if (!enablePowerBtn(true)) {
    ESP_LOGE(TAG, "enablePowerBtn failed");
    this->mark_failed();
    return;
  }
  ESP_LOGI(TAG, "ip5306 enablePowerBtn done");

  if (!setPowerBoostKeepOn(true)) {
    ESP_LOGE(TAG, "setPowerBoostKeepOn failed");
    this->mark_failed();
    return;
  }
  ESP_LOGI(TAG, "ip5306 setPowerBoostKeepOn done");

  if (!setAutoBootOnLoad(false)) {
    ESP_LOGE(TAG, "setAutoBootOnLoad failed");
    this->mark_failed();
    return;
  }
  ESP_LOGI(TAG, "ip5306 setAutoBootOnLoad done");

  if (!setLowPowerShutdownTime(64)) {
    ESP_LOGE(TAG, "setLowPowerShutdownTime failed");
    this->mark_failed();
    return;
  }
  completeChargingSetup();
}

void IP5306::completeChargingSetup() {
  uint8_t data[1];
  uint8_t value;
  // End charge current 200ma
  if (this->read_register(0x21, data, 1) == i2c::ERROR_OK) {
    value = (data[0] & 0x3f) | 0x00;
    this->write_register(0x21, &value, 1);
  } else {
    ESP_LOGE(TAG, "completeChargingSetup read 1 failed");
  }

  // Add volt 28mv
  if (this->read_register(0x22, data, 1) == i2c::ERROR_OK) {
    value = (data[0] & 0xfc) | 0x02;
    this->write_register(0x22, &value, 1);
  } else {
    ESP_LOGE(TAG, "completeChargingSetup read 2 failed");
  }

  // Vin charge CC
  if (this->read_register(0x23, data, 1) == i2c::ERROR_OK) {
    value = (data[0] & 0xdf) | 0x20;
    this->write_register(0x23, &value, 1);
  } else {
    ESP_LOGE(TAG, "completeChargingSetup read 3 failed");
  }
}

bool IP5306::setVinMaxCurrent(uint8_t cur) {
    uint8_t data[1];
    if (this->read_register(IP5306_REG_CHG_DIG, data, 1) != i2c::ERROR_OK) {
      ESP_LOGE(TAG, "setVinMaxCurrent read failed");
      return false;
    }
    uint8_t value = (data[0] & 0xE0) | cur;
    return this->write_register(IP5306_REG_CHG_DIG, &value, 1) == i2c::ERROR_OK;
}

bool IP5306::setChargeVolt(uint8_t volt) {
    uint8_t data[1];
    if (this->read_register(IP5306_REG_CHG_CTL0, data, 1) != i2c::ERROR_OK) {
      ESP_LOGE(TAG, "setChargeVolt read failed");
      return false;
    }
    uint8_t value = (data[0] & 0xFC) | volt;
    return this->write_register(IP5306_REG_CHG_CTL0, &value, 1) == i2c::ERROR_OK;
}

bool IP5306::setPowerBoostOnOff(bool enabled) {
    uint8_t data[1];
    if (this->read_register(IP5306_REG_SYS_CTL1, data, 1) != i2c::ERROR_OK) {
      ESP_LOGE(TAG, "setPowerBoostOnOff read failed");
      return false;
    }
    uint8_t value = enabled ? (data[0] | BOOST_SET_BIT) : (data[0] & (~BOOST_SET_BIT));
    return this->write_register(IP5306_REG_SYS_CTL1, &value, 1) == i2c::ERROR_OK;
}

bool IP5306::setPowerBoostSet(bool enabled) {
  uint8_t data[1];
    if (this->read_register(IP5306_REG_SYS_CTL1, data, 1) != i2c::ERROR_OK) {
      ESP_LOGE(TAG, "setPowerBoostSet read failed");
      return false;
    }
    uint8_t value = enabled ? (data[0] | SHORT_BOOST_BIT) : (data[0] & (~SHORT_BOOST_BIT));
    return this->write_register(IP5306_REG_SYS_CTL1, &value, 1) == i2c::ERROR_OK;
}

bool IP5306::setPowerVin(bool enabled) {
  uint8_t data[1];
    if (this->read_register(IP5306_REG_SYS_CTL1, data, 1) != i2c::ERROR_OK) {
      ESP_LOGE(TAG, "setPowerVin read failed");
      return false;
    }
    uint8_t value = enabled ? (data[0] | VIN_ENABLE_BIT) : (data[0] & (~VIN_ENABLE_BIT));
    return this->write_register(IP5306_REG_SYS_CTL1, &value, 1) == i2c::ERROR_OK;
}

bool IP5306::enablePowerBtn(bool enabled) {
  uint8_t data[1];
  if (this->read_register(IP5306_REG_SYS_CTL0, data, 1) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "enablePowerBtn read failed");
    return false;
  }
  uint8_t value = enabled ? (data[0] | BOOST_BUTTON_EN_BIT) : (data[0] & (~BOOST_BUTTON_EN_BIT));
  return this->write_register(IP5306_REG_SYS_CTL0, &value, 1) == i2c::ERROR_OK;
}

bool IP5306::setPowerBoostKeepOn(bool enabled) {
    uint8_t data[1];
    if (this->read_register(IP5306_REG_SYS_CTL0, data, 1) != i2c::ERROR_OK) {
      ESP_LOGE(TAG, "setPowerBoostKeepOn read failed");
      return false;
    }
    uint8_t value = enabled ? (data[0] | BOOST_OUT_BIT) : (data[0] & (~BOOST_OUT_BIT));
    return this->write_register(IP5306_REG_SYS_CTL0, &value, 1) == i2c::ERROR_OK;
}

/*
  default: true
  true: If enough load is connected, the power will turn on.
*/
bool IP5306::setAutoBootOnLoad(bool enabled) {
    uint8_t data[1];
    if (this->read_register(IP5306_REG_SYS_CTL0, data, 1) != i2c::ERROR_OK) {
      ESP_LOGE(TAG, "setAutoBootOnLoad read failed");
      return false;
    }
    uint8_t value = enabled ? (data[0] | BOOT_ON_LOAD_BIT) : (data[0] & (~BOOT_ON_LOAD_BIT));
    return this->write_register(IP5306_REG_SYS_CTL0, &value, 1) == i2c::ERROR_OK;
}

// if charge full,try set charge enable->disable->enable,can be recharged
// https://github.com/m5stack/M5Stack/blob/1e81176b2dd9cb7839f53f5a71a8143b04794de7/src/utility/Power.cpp#L270
void IP5306::setCharge(bool enabled) {
  if (this->charger_active_ == nullptr) return;
  if (this->charger_active_->has_state() && this->charger_active_->state == enabled) return;

  if (this->write_register(IP5306_REG_SYS_CTL0, &IP5306_REG_CHARGE_OUT_BIT, enabled ? 1 : 0) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "set charge failed %d", enabled);
    this->mark_failed();
  }
  ESP_LOGE(TAG, "set charge worked, %d", enabled);
  if (!this->charger_active_->has_state() || (this->charger_active_->state != enabled))
    this->charger_active_->publish_state(enabled);
}

bool IP5306::setLowPowerShutdownTime(int time) {
  uint8_t data[1];
  if (this->read_register(IP5306_REG_SYS_CTL2, data, 1) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "setLowPowerShutdownTime read failed");
    return false;
  }
  uint8_t value;
  switch (time) {
    case 8:
      value = ((data[0] & (~SHUTDOWNTIME_MASK)) | SHUTDOWNTIME_8S);
      break;
    case 16:
      value = ((data[0] & (~SHUTDOWNTIME_MASK)) | SHUTDOWNTIME_16S);
      break;
    case 32:
      value = ((data[0] & (~SHUTDOWNTIME_MASK)) | SHUTDOWNTIME_32S);
      break;
    case 64:
      value = ((data[0] & (~SHUTDOWNTIME_MASK)) | SHUTDOWNTIME_64S);
      break;
    default:
      ESP_LOGE(TAG, "setLowPowerShutdownTime invalid time");
      return false;
  }
  return this->write_register(IP5306_REG_SYS_CTL2, &value, 1) == i2c::ERROR_OK;
}

bool IP5306::readBatteryLevel(uint8_t* data) {
  if (this->battery_level_ == nullptr) return false;
  if (this->read_register(IP5306_REG_LEVEL, data, 1) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "unable to read level");
    this->mark_failed();
    return false;
  }
  float value = 0;
  switch (data[0] & 0xF0) {
    case 0xE0: value = 25; break;
    case 0xC0: value = 50; break;
    case 0x80: value = 75; break;
    case 0x00: value = 100; break;
  }
  if (!this->battery_level_->has_state() || (this->battery_level_->state != value))
    this->battery_level_->publish_state(value);
  return true;
}

bool IP5306::readBatteryStatus(uint8_t* data) {
  if (this->read_register(IP5306_REG_READ0, data, 2) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "unable to read status");
    this->mark_failed();
    return false;
  }
  if (this->charger_connected_ != nullptr)
    this->charger_connected_->publish_state(data[0] & 0x08);
  if (this->charge_full_ != nullptr)
    this->charge_full_->publish_state(data[1] & 0x08);

  if (this->charger_connected_->state && !this->charge_full_->state) {
    this->setCharge(true);
  } else {
    this->setCharge(false);
  }
  return true;
}

void IP5306::loop() {
  uint8_t data[2];
  if (!readBatteryLevel(data)) return;
  if (!readBatteryStatus(data)) return;
}

}  // namespace ip5306
}  // namespace esphome

