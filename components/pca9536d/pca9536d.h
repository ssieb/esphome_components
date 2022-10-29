#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace pca9536d {

class PCA9536D: public Component, public i2c::I2CDevice {
 public:
  void setup() override;
  float get_setup_priority() const { return setup_priority::IO; }
  void dump_config() override;

  bool digital_read(uint8_t pin);
  void digital_write(uint8_t pin, bool value);
  void set_pin_mode(uint8_t pin, uint8_t mode);

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
  void setup() override;
  void pin_mode(gpio::Flags flags) override;
  bool digital_read() override;
  void digital_write(bool value) override;
  std::string dump_summary() const override;

  void set_parent(PCA9536D *parent) { parent_ = parent; }
  void set_pin(uint8_t pin) { pin_ = pin; }
  void set_inverted(bool inverted) { inverted_ = inverted; }
  void set_flags(gpio::Flags flags) { flags_ = flags; }

 protected:
  PCA9536D *parent_;
  uint8_t pin_;
  bool inverted_;
  gpio::Flags flags_;
};

}  // namespace pca9536d
}  // namespace esphome
