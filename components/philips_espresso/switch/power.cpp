#include "esphome/core/log.h"
#include "power.h"

namespace esphome
{
    namespace philips_series_3200
    {
        namespace philips_power_switch
        {

            static const char *TAG = "philips_power_switch";

            void Power::setup()
            {
            }

            void Power::loop()
            {
            }

            void Power::write_state(bool state)
            {
                if (state)
                {
                    // Send pre-power on message
                    for (unsigned int i = 0; i <= MESSAGE_REPETITIONS; i++)
                        // mainboard_uart_->write_array({0xD5, 0x55, 0x0A, 0x01, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x0E, 0x12});
                        mainboard_uart_->write_array({0xD5, 0x55, 0x0A, 0x01, 0x03, 0x00, 0x12, 0x00, 0x00, 0x00, 0x0F, 0x05});

                    // Send power on message
                    if (cleaning_)
                    {
                        // Send power on command with cleaning
                        for (unsigned int i = 0; i <= MESSAGE_REPETITIONS; i++)
                            // mainboard_uart_->write_array({0xD5, 0x55, 0x02, 0x01, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x38, 0x15});
                            mainboard_uart_->write_array({0xD5, 0x55, 0x02, 0x01, 0x03, 0x00, 0x12, 0x00, 0x00, 0x00, 0x39, 0x02});
                   }
                    else
                    {
                        // Send power on command without cleaning
                        for (unsigned int i = 0; i <= MESSAGE_REPETITIONS; i++)
                            // mainboard_uart_->write_array({0xD5, 0x55, 0x01, 0x01, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x25, 0x27});
                            mainboard_uart_->write_array({0xD5, 0x55, 0x01, 0x01, 0x03, 0x00, 0x12, 0x00, 0x00, 0x00, 0x25, 0x27});
                    }
// // https://github.com/mkorenko/esp-phillips-3200/blob/main/esp-phillips-3200.cpp

// byte Phillips3200::_buf_power_on[12] = {
//   0xD5, 0x55, 0x0A, 0x01, 0x03, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x2A, 0x10
// };
// byte Phillips3200::_buf_power_on1[12] = {
//   0xD5, 0x55, 0x02, 0x01, 0x03, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x1C, 0x17
// };
// byte Phillips3200::_buf_power_off[12] = {
//   0xD5, 0x55, 0x00, 0x01, 0x03, 0x00, 0x0E, 0x01, 0x00, 0x00, 0x39, 0x39
// };
// byte Phillips3200::_buf_request_info[12] = {
//   0xD5, 0x55, 0x00, 0x01, 0x03, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x35, 0x34
// };

                    mainboard_uart_->flush();

                    // Perform power trip (invert state twice)
                    power_pin_->digital_write(!power_pin_->digital_read());
                    delay(POWER_TRIP_DELAY);
                    power_pin_->digital_write(!power_pin_->digital_read());
                }
                else
                {
                    // Send power off message
                    for (unsigned int i = 0; i <= MESSAGE_REPETITIONS; i++)
                     // mainboard_uart_->write_array({0xD5, 0x55, 0x00, 0x01, 0x03, 0x00, 0x0E, 0x01, 0x00, 0x00, 0x39, 0x39}); //2200
                        mainboard_uart_->write_array({0xD5, 0x55, 0x00, 0x01, 0x03, 0x00, 0x12, 0x01, 0x00, 0x00, 0x1C, 0x2C});  //3200
                    mainboard_uart_->flush();
                }

                // The state will be published once the display starts sending messages
            }

            void Power::dump_config()
            {
                ESP_LOGCONFIG(TAG, "Philips Series 3200 Power Switch");
            }

        } // namespace power_switch
    }     // namespace philips_series_3200
} // namespace esphome