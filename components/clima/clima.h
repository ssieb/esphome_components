#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace clima {

class Clima : public Component,  public uart::UARTDevice {
 public:
  float get_setup_priority() const override { return setup_priority::LATE; }
  void loop() override;
  void dump_config() override;

  void set_time_sensor(text_sensor::TextSensor *text_sensor) { this->time_sensor_ = text_sensor; }
  void set_brightness_east_sensor(sensor::Sensor *sensor) { this->brightness_east_sensor_ = sensor; }
  void set_brightness_west_sensor(sensor::Sensor *sensor) { this->brightness_west_sensor_ = sensor; }
  void set_brightness_south_sensor(sensor::Sensor *sensor) { this->brightness_south_sensor_ = sensor; }
  void set_twilight_sensor(sensor::Sensor *sensor) { this->twilight_sensor_ = sensor; }
  void set_humidity_sensor(sensor::Sensor *sensor) { this->humidity_sensor_ = sensor; }
  void set_temperature_sensor(sensor::Sensor *sensor) { this->temperature_sensor_ = sensor; }
  void set_precipitation_sensor(binary_sensor::BinarySensor *bsensor) { this->precipitation_bsensor_ = bsensor; }
  void set_windspeed_sensor(sensor::Sensor *sensor) { this->windspeed_sensor_ = sensor; }
  void set_low_voltage_sensor(binary_sensor::BinarySensor *bsensor) { this->low_voltage_bsensor_ = bsensor; }
  void set_no_time_sync_sensor(binary_sensor::BinarySensor *bsensor) { this->no_time_sync_bsensor_ = bsensor; }
  void set_heating_sensor(binary_sensor::BinarySensor *bsensor) { this->heating_bsensor_ = bsensor; }
  void set_time_syncing_sensor(binary_sensor::BinarySensor *bsensor) { this->time_syncing_bsensor_ = bsensor; }

 protected:
  text_sensor::TextSensor *time_sensor_{nullptr};
  sensor::Sensor *brightness_east_sensor_{nullptr};
  sensor::Sensor *brightness_west_sensor_{nullptr};
  sensor::Sensor *brightness_south_sensor_{nullptr};
  sensor::Sensor *twilight_sensor_{nullptr};
  sensor::Sensor *humidity_sensor_{nullptr};
  sensor::Sensor *temperature_sensor_{nullptr};
  binary_sensor::BinarySensor *precipitation_bsensor_{nullptr};
  sensor::Sensor *windspeed_sensor_{nullptr};
  binary_sensor::BinarySensor *low_voltage_bsensor_{nullptr};
  binary_sensor::BinarySensor *no_time_sync_bsensor_{nullptr};
  binary_sensor::BinarySensor *heating_bsensor_{nullptr};
  binary_sensor::BinarySensor *time_syncing_bsensor_{nullptr};

  bool reading_{false};
  std::vector<uint8_t> buffer_;
  void handle_data_(uint8_t *data);
};

}  // namespace clima
}  // namespace esphome
