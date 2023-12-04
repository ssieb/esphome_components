#pragma once

#include "../ht16k33_base/ht16k33_display.h"

namespace esphome {
namespace ht16k33 {

class HT16K33AlphaDisplay : public HT16K33BaseDisplay {
 protected:
  void display_() override;
  uint16_t read_character_(uint8_t c) const override;
  uint16_t decimal_point_mask_() const override { return 0x4000; }
  bool supports_colon_() const override { return false; }
};

}  // namespace ht16k33
}  // namespace esphome