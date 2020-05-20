#pragma once

#include "esphome/core/automation.h"
#include "esphome/components/keypad/keypad.h"
#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome {
namespace keypad {

class KeypadTextSensor : public KeypadListener, public text_sensor::TextSensor, public Component {
 public:
  KeypadTextSensor();
  void dump_config() override;
  void set_max_length(int max_length) { this->max_length_ = max_length; };
  void set_end_keys(std::string end_keys) { this->end_keys_ = end_keys; };
  void set_back_keys(std::string back_keys) { this->back_keys_ = back_keys; };
  void set_allowed_keys(std::string allowed_keys) { this->allowed_keys_ = allowed_keys; };
  Trigger<std::string> *get_progress_trigger() const;

  void key_pressed(unsigned char key) override;

 protected:
  int max_length_ = 0;
  std::string end_keys_;
  std::string back_keys_;
  std::string allowed_keys_;
  std::string result_;
  Trigger<std::string> *progress_trigger_;
};

}  // namespace keypad
}  // namespace esphome

