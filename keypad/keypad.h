#pragma once

#include "esphome/core/component.h"
#include "esphome/core/esphal.h"
#include "esphome/core/helpers.h"
#include <stdlib.h>

namespace esphome {
namespace keypad {

class KeypadListener {
 public:
  virtual void button_pressed(int row, int col) {};
  virtual void button_released(int row, int col) {};
  virtual void key_pressed(unsigned char key) {};
  virtual void key_released(unsigned char key) {};
};

class Keypad : public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;
  void set_columns(std::vector<GPIOPin *> pins) { columns_ = pins; };
  void set_rows(std::vector<GPIOPin *> pins) { rows_ = pins; };
  void set_keys(std::string keys) { keys_ = keys; };
  void set_debounce_time(int debounce_time) { debounce_time_ = debounce_time; };

  void register_listener(KeypadListener *listener);

 protected:
  std::vector<GPIOPin *> rows_;
  std::vector<GPIOPin *> columns_;
  std::string keys_;
  int debounce_time_ = 0;
  int pressed_key_ = -1;

  std::vector<KeypadListener *> listeners_{};
};

}  // namespace keypad
}  // namespace esphome


