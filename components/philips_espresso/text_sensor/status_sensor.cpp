// https://github.com/mkorenko/esp-phillips-3200/blob/main/esp-phillips-3200.cpp

#include "esphome/core/log.h"
#include "status_sensor.h"

namespace esphome
{
    namespace philips_series_3200
    {
        namespace philips_status_sensor
        {
            static const char *TAG = "philips_status_sensor";

            void StatusSensor::setup()
            {
            }

            void StatusSensor::dump_config()
            {
                ESP_LOGCONFIG(TAG, "Philips Status Text Sensor");
            }

            void StatusSensor::update_status(uint8_t *data, size_t len)
            {
                // reject invalid messages
                if (len < 19 && data[0] != 0xD5 && data[1] != 0x55)
                    return;

                // TODO: figure out how the checksum is calculated and only parse valid messages

                // Check if the play/pause button is on/off/blinking
                if ((data[16] == 0x07) != play_pause_led_)
                {
                    play_pause_last_change_ = millis();
                }
                play_pause_led_ = data[16] == 0x07;

                // Check for idle state (selection led on)
                //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18
                // D5 55 00 07 07 07 3F 38 00 00 00 00 38 00 00 00 00 31 06
                if (data[3] == 0x07 && data[4] == 0x07 && data[5] == 0x07)
                // if (data[3] == 0x01 && data[4] == 0x03 && data[5] == 0x00 && data[6] == 0x12)
                {
                    // selecting a beverage can result in a short "busy" period since the play/pause button has not been blinking
                    // This can be circumvented: if the user is on the selection screen/idle we can reset the timer
                    play_pause_last_change_ = millis();

                    update_state("Idle");
                    return;
                }

                // Check for rotating icons - pre heating
                // D5 55 00 00 00 03 38 00 00 00 00 00 00 00 00 00 00 2C 1B
                if (data[3] == 0x03 || data[4] == 0x03 || data[5] == 0x03 || data[6] == 0x03)
                {
                    if (play_pause_led_)
                        update_state("Cleaning");
                    else
                        update_state("Preparing");
                    return;
                }


                // Water empty led
                if (data[14] == 0x38)
                {
                    update_state("Water empty");
                    return;
                }

                // Waste container led
                if (data[15] == 0x07)
                {
                    update_state("Waste container warning");
                    return;
                }

                // Warning/Error led
                if (data[15] == 0x38)
                {
                    update_state("Error");
                    return;
                }

                // Coffee selected
                // D5 55 00 00 00 07 00 00 38 07 38 07 00 00 00 00 07 33 15
                if (data[5] == 0x07)
                {
                    if (millis() - play_pause_last_change_ < BLINK_THRESHOLD)
                        update_state("Coffee selected");
                    else
                        update_state("Busy");
                    return;
                }

                if (data[5] == 0x38)
                {
                    if (millis() - play_pause_last_change_ < BLINK_THRESHOLD)
                        update_state("Double Coffee selected");
                    else
                        update_state("Busy");
                    return;
                }

                // Latte selected
                if (data[6] == 0x07)
                {
                    if (millis() - play_pause_last_change_ < BLINK_THRESHOLD)
                        update_state("Latte selected");
                    else
                        update_state("Busy");
                    return;
                }

                // Cappuccino selected
                if (data[4] == 0x07)
                {
                    if (millis() - play_pause_last_change_ < BLINK_THRESHOLD)
                        update_state("Cappuccino selected");
                    else
                        update_state("Busy");
                    return;
                }

                // Espresso selected
                if (data[3] == 0x07)
                {
                    if (millis() - play_pause_last_change_ < BLINK_THRESHOLD)
                        update_state("Espresso selected");
                    else
                        update_state("Busy");
                    return;
                }

                if (data[3] == 0x38)
                {
                    if (millis() - play_pause_last_change_ < BLINK_THRESHOLD)
                        update_state("Double Espresso selected");
                    else
                        update_state("Busy");
                    return;
                }

                // Americano selected
                //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18
                // D5 55 00 00 00 00 38 00 38 07 38 07 00 00 00 00 00 2A 1C
                if (data[6] == 0x38)
                {
                    if (millis() - play_pause_last_change_ < BLINK_THRESHOLD)
                        update_state("Americano selected");
                    else
                        update_state("Busy");
                    return;
                }

                if (data[7] == 0x07)
                {
                    if (millis() - play_pause_last_change_ < BLINK_THRESHOLD)
                        update_state("Double Americano selected");
                    else
                        update_state("Busy");
                    return;
                }

                // Hot Water selected
                //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18
                // D5 55 00 00 00 00 00 38 00 00 3F 07 00 00 00 00 07 3D 2E 
                if (data[7] == 0x38)
                {
                    if (millis() - play_pause_last_change_ < BLINK_THRESHOLD)
                        update_state("Hot Water selected");
                    else
                        update_state("Busy");
                    return;
                }
            }

        } // namespace philips_status_sensor
    } // namespace philips_series_3200
} // namespace esphome