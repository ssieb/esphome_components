#pragma once

#include "esphome/components/i2c/i2c.h"
#include "esphome/components/output/float_output.h"
#include "esphome/core/component.h"

namespace esphome {
namespace pca963x {

class PCA963xOutput;

class PCA963xChannel : public output::FloatOutput {
public:
  PCA963xChannel(PCA963xOutput *parent, uint8_t channel)
      : parent_(parent), channel_(channel) {}

protected:
  void write_state(float state) override;

  PCA963xOutput *parent_;
  uint8_t channel_;
};

/// PCA9634 float output component
class PCA963xOutput : public Component, public i2c::I2CDevice {
public:
  PCA963xOutput();

  PCA963xChannel *create_channel(uint8_t channel);

  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::HARDWARE; }
  void loop() override;
  void set_blinking_mode(bool mode) {
    this->blink_mode_ = mode;
    this->update_ = true;
  }
  void set_output_mode(bool mode) {
    this->output_mode_ = mode;
    this->update_ = true;
  }
  void set_inverted(boolean inverted) {
    this->inverted_ = inverted;
    this->update_ = true;
  }

  /**
   * Switch to normal mode.
   */
  void wakeUp();

  /**
   * Switch to low power mode.
   */
  void sleep();


protected:
  friend PCA963xChannel;

  void set_channel_value_(uint8_t channel, uint8_t value) {
    if (channel >= this->max_channels)
      return;
    if (this->pwm_values_[channel] != value) {
      this->update_ = true;
      this->pwm_values_[channel] = value;
    }
  }

  bool blink_mode_{false};
  bool output_mode_{false};
  bool inverted_{false};

  uint8_t pwm_values_[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t max_channels_{0};

  bool update_{false};
};

class PCA9633Output : public PCA963xOutput {
public:
  PCA9633Output() : max_channels_(4){};
};

class PCA9634Output : public PCA963xOutput {
public:
  PCA9634Output() : max_channels_(8){};
};

class PCA9635Output : public PCA963xOutput {
public:
  PCA9635Output() : max_channels_(16){};
};

} // namespace pca963x
} // namespace esphome
