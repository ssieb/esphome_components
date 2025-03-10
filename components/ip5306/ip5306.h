#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace ip5306 {

class IP5306 : public i2c::I2CDevice, public Component {
 public:
  void setup() override;
  void loop() override;

  float get_setup_priority() const override;

  void set_battery_level(sensor::Sensor *sensor) { this->battery_level_ = sensor; }
  void set_charger_connected(binary_sensor::BinarySensor *sensor) { this->charger_connected_ = sensor; }
  void set_charger_active(binary_sensor::BinarySensor *sensor) { this->charger_active_ = sensor; }
  void set_charge_full(binary_sensor::BinarySensor *sensor) { this->charge_full_ = sensor; }

  void set_power_boost_on(bool enabled) { this->powerBoostOn_ = enabled; }
  void set_power_boost_set(bool enabled) { this->powerBoostSet_ = enabled; }
  void set_power_vin(bool enabled) { this->powerVin_ = enabled; }
  void set_power_btn(bool enabled) { this->powerBtn_ = enabled; }
  void set_power_boost_keep_on(bool enabled) { this->powerBoostKeepOn_ = enabled; }
  void set_auto_boot_on_load(bool enabled) { this->autoBootOnLoad_ = enabled; }
  void set_enable_power_btn(bool enabled) { this->enablePowerBtn_ = enabled; }
  void set_low_power_shutdown_time(int time) { this->lowPowerShutdownTime_ = time; }

 protected:
  sensor::Sensor *battery_level_{nullptr};
  binary_sensor::BinarySensor *charger_connected_{nullptr};
  binary_sensor::BinarySensor *charge_full_{nullptr};
  binary_sensor::BinarySensor *charger_active_{nullptr};

  void setCharge(bool enabled);
  bool readBatteryLevel(uint8_t* data);
  bool readBatteryStatus(uint8_t* data);
  bool setVinMaxCurrent(uint8_t cur);
  bool setChargeVolt(uint8_t volt);
  bool setPowerBoostOnOff(bool enabled);
  bool setPowerBoostSet(bool enabled);
  bool setPowerVin(bool enabled);
  bool enablePowerBtn(bool enabled);
  bool setPowerBoostKeepOn(bool enabled);
  bool setAutoBootOnLoad(bool enabled);
  bool setLowPowerShutdownTime(int time);
  void completeChargingSetup();

  private:
    bool powerBoostOn_{false};
    bool powerBoostSet_{false};
    bool powerVin_{false};
    bool powerBtn_{false};
    bool powerBoostKeepOn_{false};
    bool autoBootOnLoad_{false};
    bool enablePowerBtn_{false};
    int lowPowerShutdownTime_{0};
};

}  // namespace ip5306
}  // namespace esphome

