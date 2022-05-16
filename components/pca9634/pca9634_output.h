#pragma once

#include "esphome/core/component.h"
#include "esphome/components/output/float_output.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace pca9634 {

/// Inverts polarity of channel output signal
extern const uint8_t PCA9685_MODE_INVERTED;
/// Channel update happens upon ACK (post-set) rather than on STOP (endTransmission)
extern const uint8_t PCA9685_MODE_OUTPUT_ONACK;
/// Use a totem-pole (push-pull) style output rather than an open-drain structure.
extern const uint8_t PCA9685_MODE_OUTPUT_TOTEM_POLE;
/// For active low output enable, sets channel output to high-impedance state
extern const uint8_t PCA9685_MODE_OUTNE_HIGHZ;
/// Similarly, sets channel output to high if in totem-pole mode, otherwise
extern const uint8_t PCA9685_MODE_OUTNE_LOW;

enum PCA9634GroupMode {
  PCA9634_MODE2_DIM = 0x00,
  PCA9634_MODE2_BLINK = 0x20,
};

enum PCA9634OutputMode {
  PCA9634_MODE2_OPEN_DRAIN = 0x00,
  PCA9634_MODE2_TOTEM_POLE = 0x04,
};

class PCA9634Output;

class PCA9634Channel : public output::FloatOutput {
 public:
  PCA9634Channel(PCA9634Output *parent, uint8_t channel) : parent_(parent), channel_(channel) {}

 protected:
  void write_state(float state) override;

  PCA9634Output *parent_;
  uint8_t channel_;
};

/// PCA9634 float output component
class PCA9634Output : public Component, public i2c::I2CDevice {
 public:
  PCA9685Output();

  PCA9685Channel *create_channel(uint8_t channel);

  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::HARDWARE; }
  void loop() override;
  void set_group_mode(PCA9634GroupMode mode) { this->group_mode_ = mode; }
  void set_output_mode(PCA9634OutputMode mode) { this->output_mode_ = mode; }
  void set_inverted(boolean inverted) { this->inverted_ = inverted; }

 protected:
  friend PCA9634Channel;

  void set_channel_value_(uint8_t channel, uint8_t value) {
    if (this->pwm_values_[channel] != value) {
      this->update_ = true;
      this->pwm_values_[channel] = value;
    }
  }

  PCA9634GroupMode group_mode_{PCA9634_MODE2_DIM};
  PCA9634OutputMode output_mode_{PCA9634_MODE2_TOTEM_POLE};
  boolean inverted_{false};

  uint8_t pwm_values_[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t grppwm_{0};
  uint8_t grpfreq_{0};
  bool update_{false};
};

}  // namespace pca9634
}  // namespace esphome
