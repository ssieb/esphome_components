#pragma once

#include "../vbus.h"
#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome {
namespace vbus {

using message_handler_t = std::function<void(std::vector<uint8_t> &)>;

class DeltaSol_C_tsensor : public VBusListener, public Component {
 public:
  void dump_config() override;
  void set_time_tsensor(text_sensor::TextSensor *tsensor) { this->time_tsensor_ = tsensor; }

 protected:
  static constexpr const char *TAG_ = "vbus.deltasol_c.text";
  text_sensor::TextSensor *time_tsensor_{nullptr};

  void handle_message_(std::vector<uint8_t> &message) override;
};

class DeltaSol_CS2_tsensor : public VBusListener, public Component {
 public:
  void dump_config() override;
  void set_version_tsensor(text_sensor::TextSensor *tsensor) { this->version_tsensor_ = tsensor; }

 protected:
  static constexpr const char *TAG_ = "vbus.deltasol_cs2.text";
  text_sensor::TextSensor *version_tsensor_{nullptr};

  void handle_message_(std::vector<uint8_t> &message) override;
};

class DeltaSol_BS_Plus_tsensor : public VBusListener, public Component {
 public:
  void dump_config() override;
  void set_time_tsensor(text_sensor::TextSensor *tsensor) { this->time_tsensor_ = tsensor; }
  void set_version_tsensor(text_sensor::TextSensor *tsensor) { this->version_tsensor_ = tsensor; }

 protected:
  static constexpr const char *TAG_ = "vbus.deltasol_bs_plus.text";
  text_sensor::TextSensor *time_tsensor_{nullptr};
  text_sensor::TextSensor *version_tsensor_{nullptr};

  void handle_message_(std::vector<uint8_t> &message) override;
};

}  // namespace vbus
}  // namespace esphome
