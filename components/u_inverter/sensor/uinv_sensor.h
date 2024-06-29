#pragma once

#include "../u_inverter.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace u_inverter {

class UInverterSensor : public UInverterListener, public Component {
 public:
  void dump_config() override;
  void set_fault_code_sensor(sensor::Sensor *sensor) { this->fault_code_sensor_ = sensor; }
  void set_mains_voltage_sensor(sensor::Sensor *sensor) { this->mains_voltage_sensor_ = sensor; }
  void set_mains_frequency_sensor(sensor::Sensor *sensor) { this->mains_frequency_sensor_ = sensor; }
  void set_mains_lossv_high_sensor(sensor::Sensor *sensor) { this->mains_lossv_high_sensor_ = sensor; }
  void set_mains_lossv_low_sensor(sensor::Sensor *sensor) { this->mains_lossv_low_sensor_ = sensor; }
  void set_mains_lossf_high_sensor(sensor::Sensor *sensor) { this->mains_lossf_high_sensor_ = sensor; }
  void set_mains_lossf_low_sensor(sensor::Sensor *sensor) { this->mains_lossf_low_sensor_ = sensor; }
  void set_mains_power_sensor(sensor::Sensor *sensor) { this->mains_power_sensor_ = sensor; }

 protected:
  sensor::Sensor *fault_code_sensor_{nullptr};
  sensor::Sensor *mains_voltage_sensor_{nullptr};
  sensor::Sensor *mains_frequency_sensor_{nullptr};
  sensor::Sensor *mains_lossv_high_sensor_{nullptr};
  sensor::Sensor *mains_lossv_low_sensor_{nullptr};
  sensor::Sensor *mains_lossf_high_sensor_{nullptr};
  sensor::Sensor *mains_lossf_low_sensor_{nullptr};
  sensor::Sensor *mains_power_sensor_{nullptr};

  void handle_message(UInverterCmd cmd, std::string &data) override;
};

}  // namespace u_inverter
}  // namespace esphome
