#include "desky_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace desky {

static const char *TAG = "desky.sensor";

void DeskySensor::loop() {
  static int state = 0;
  static uint8_t high_byte;

  while (this->available()) {
    uint8_t c;
    int value;
    this->read_byte(&c);
    switch (state) {
     case 0:
      if (c == 1)
	state = 1;
      break;
     case 1:
      if (c == 1)
	state = 2;
      else
	state = 0;
      break;
     case 2:
      high_byte = c;
      state = 3;
      break;
     case 3:
      value = (high_byte << 8) + c;
      this->publish_state(value);
      state = 0;
      break;
    }
  }
}

void DeskySensor::dump_config() { LOG_SENSOR("", "Desky Sensor", this); }

}  // namespace desky
}  // namespace esphome
