#pragma once

#include "esphome/core/component.h"
#include "esphome/core/esphal.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace pca9536d {

enum PCA9536DGPIOMode : uint8_t {
  PCA9536D_INPUT = INPUT,
  PCA9536D_OUTPUT = OUTPUT,
};

class PCA9536DComponent : public Component, public i2c::I2CDevice {
 public:
  void setup() override;
  bool digital_read(uint8_t pin);
  void digital_write(uint8_t pin, bool value);
  void pin_mode(uint8_t pin, uint8_t mode);

  float get_setup_priority() const { return setup_priority::IO; }

  void dump_config() override;

 protected:
  bool read_gpio_();
  bool write_gpio_();
  bool set_modes_();

  // pin modes - 0 means output, 1 means input
  uint8_t modes_{0xff};
  uint8_t outputs_{0x00};
  uint8_t inputs_{0x00};
};

class PCA9536DGPIOPin : public GPIOPin {
 public:
  PCA9536DGPIOPin(PCA9536DComponent *parent, uint8_t pin, uint8_t mode, bool inverted = false);

  void setup() override;
  void pin_mode(uint8_t mode) override;
  bool digital_read() override;
  void digital_write(bool value) override;

 protected:
  PCA9536DComponent *parent_;
};

}  // namespace pca9536d
}  // namespace esphome
