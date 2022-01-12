#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace mcp342x {

class MCP342XSensor;

class MCP342XComponent : public Component, public i2c::I2CDevice {
 public:
  void register_sensor(MCP342XSensor *obj) { this->sensors_.push_back(obj); }
  void setup() override;
  void loop() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  /// Helper method to request a measurement from a sensor.
  void request_measurement(MCP342XSensor *sensor);

 protected:
  std::vector<MCP342XSensor *> sensors_;
  bool busy_ = false;
  std::vector<MCP342XSensor *> queue_;
};

class MCP342XSensor : public sensor::Sensor, public PollingComponent {
 public:
  MCP342XSensor(MCP342XComponent *parent) : parent_(parent) {}
  void update() override;
  void set_channel(int channel) { channel_ = channel; }
  void set_gain(int gain) { gain_ = gain; }
  void set_resolution(int resolution) { resolution_ = resolution; }

  uint8_t get_channel() const { return channel_; }
  uint8_t get_gain() const { return gain_; }
  uint8_t get_resolution() const { return resolution_; }

  void update_result(float value);

 protected:
  MCP342XComponent *parent_;
  int channel_;
  int gain_;
  int resolution_;
  bool busy_ = false;
};

}  // namespace mcp342x
}  // namespace esphome
