#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace reinhardt {

enum SensorType : uint8_t {
  RS_TEMPERATURE,
  RS_HUMIDITY,
  RS_IN_TEMPERATURE,
  RS_PRESSURE,
  RS_DIRECTION,
  RS_PREVAILING,
  RS_WINDSPEED,
  RS_PEAKSPEED,
  RS_AVGSPEED,
  RS_RAIN,
  RS_RAIN_INTERVAL,
  RS_RADIATION,
  RS_ADDSENSOR1,
  RS_ADDSENSOR2,
  RS_ADDSENSOR3,
  RS_ADDSENSOR4,
  RS_ADDSENSOR5,
  RS_ADDSENSOR6,
  RS_UVRADIATION,
  RS_BRIGHTNESS,
  RS_BAROMETER,
  RS_DEWPOINT,
  RS_WINDCHILL,
  RS_ALTITUDE,
  RS_GPS_ALTITUDE,
  RS_GPS_LATITUDE,
  RS_GPS_LONGITUDE,
  RS_GPS_SPEED,
  RS_GPS_SATELLITES,
  RS_MAX
};

class Reinhardt : public Component,  public uart::UARTDevice {
 public:
  float get_setup_priority() const override { return setup_priority::DATA; }
  void loop() override;
  void dump_config() override;

  void add_sensor(SensorType type, sensor::Sensor *sens) {
    this->sensors_[type] = sens;
  }

 protected:
  void parse_values_();
  std::vector<uint8_t> rx_message_;
  std::array<sensor::Sensor *, 30> sensors_{};
};

}  // namespace reinhardt
}  // namespace esphome
