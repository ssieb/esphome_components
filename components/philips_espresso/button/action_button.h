#pragma once

#include "../philips_series_3200.h"
#include "esphome/components/button/button.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace philips_series_3200 {

/**
 * @brief Emulates (a) button press(es) using the mainboard uart.
 *
 */
class ActionButton : public button::Button, public Parented<PhilipsSeries3200> {
 public:
  void dump_config() override;
  /**
   * @brief Set the action used by this ActionButton.
   *
   * @param action Action to use
   */
  void set_action(Action action) { action_ = action; };

 private:
  /**
   * @brief Executes button press
   *
   */
  void press_action() override;

  /// @brief Action used by this Button
  Action action_;
};

} // namespace philips_series_3200
} // namespace esphome
