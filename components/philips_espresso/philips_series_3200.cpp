#include "esphome/core/log.h"
#include "philips_series_3200.h"

#define BUFFER_SIZE 32

namespace esphome {
namespace philips_series_3200 {

static const char const *TAG = "philips_series_3200";
static const int MESSAGE_REPETITIONS = 5;
static const int BUTTON_SEQUENCE_DELAY = 100;

static const uint8_t const *MSG_SELECT_COFFEE = {0xD5, 0x55, 0x00, 0x01, 0x03, 0x00, 0x0E, 0x08, 0x00, 0x00, 0x1D, 0x1E};
static const uint8_t const *MSG_SELECT_ESPRESSO = {0xD5, 0x55, 0x00, 0x01, 0x03, 0x00, 0x0E, 0x02, 0x00, 0x00, 0x2D, 0x2F};
static const uint8_t const *MSG_SELECT_AMERICANO = {0xD5, 0x55, 0x00, 0x01, 0x03, 0x00, 0x0E, 0x20, 0x00, 0x00, 0x04, 0x15};
static const uint8_t const *MSG_SELECT_CAPPUCCINO = {0xD5, 0x55, 0x00, 0x01, 0x03, 0x00, 0x0E, 0x04, 0x00, 0x00, 0x05, 0x03};
static const uint8_t const *MSG_SELECT_LATTE = {0xD5, 0x55, 0x00, 0x01, 0x03, 0x00, 0x0E, 0x10, 0x00, 0x00, 0x2D, 0x24};
static const uint8_t const *MSG_SELECT_HOT_WATER = {0xD5, 0x55, 0x00, 0x01, 0x03, 0x00, 0x0E, 0x00, 0x01, 0x00, 0x39, 0x38};
static const uint8_t const *MSG_SELECT_BEAN_AMOUNT = {0xD5, 0x55, 0x00, 0x01, 0x03, 0x00, 0x0E, 0x00, 0x02, 0x00, 0x2D, 0x2D};
static const uint8_t const *MSG_SELECT_WATER_AMOUNT = {0xD5, 0x55, 0x00, 0x01, 0x03, 0x00, 0x0E, 0x00, 0x04, 0x00, 0x04, 0x07};
static const uint8_t const *MSG_SELECT_FROTHED_MILK_AMOUNT = {0xD5, 0x55, 0x00, 0x01, 0x03, 0x00, 0x0E, 0x00, 0x08, 0x00, 0x1F, 0x16};
static const uint8_t const *MSG_SELECT_AQUA_CLEAN = {0xD5, 0x55, 0x00, 0x01, 0x03, 0x00, 0x0E, 0x00, 0x10, 0x00, 0x29, 0x34};
static const uint8_t const *MSG_SELECT_CALC_CLEAN = {0xD5, 0x55, 0x00, 0x01, 0x03, 0x00, 0x0E, 0x00, 0x20, 0x00, 0x0C, 0x35};
static const uint8_t const *MSG_PLAY_PAUSE = {0xD5, 0x55, 0x00, 0x01, 0x03, 0x00, 0x0E, 0x00, 0x00, 0x01, 0x3D, 0x30};

// // https://github.com/micki88/Philips-ep3200-testing/blob/main/withWebserver_EP3200_Philips.ino

// // BYTE ESPRESSO[] =       {0XD5, 0X55, 0X00, 0X01, 0X03, 0X00, 0X0E, 0X02, 0X00, 0X00, 0X2D, 0X2F};
// // BYTE COFFEE[] =         {0XD5, 0X55, 0X00, 0X01, 0X03, 0X00, 0X0E, 0X08, 0X00, 0X00, 0X1D, 0X1E};
// // BYTE AMERICANO[] =      {0XD5, 0X55, 0X00, 0X01, 0X03, 0X00, 0X0E, 0X20, 0X00, 0X00, 0X04, 0X15};
// // BYTE CAPPUCCINO[] =     {0XD5, 0X55, 0X00, 0X01, 0X03, 0X00, 0X0E, 0X04, 0X00, 0X00, 0X05, 0X03};
// // BYTE LATTEMACCHIATO[] = {0XD5, 0X55, 0X00, 0X01, 0X03, 0X00, 0X0E, 0X10, 0X00, 0X00, 0X2D, 0X24};
// // BYTE HOTWATER[] =       {0XD5, 0X55, 0X00, 0X01, 0X03, 0X00, 0X0E, 0X00, 0X01, 0X00, 0X39, 0X38};
// // BYTE COFFEEGROUNDS[] =   {0XD5, 0X55, 0X00, 0X01, 0X03, 0X00, 0X0E, 0X00, 0X02, 0X00, 0X2D, 0X2D};
// BYTE COFFEEWATER[] =    {0XD5, 0X55, 0X00, 0X01, 0X03, 0X00, 0X0E, 0X00, 0X04, 0X00, 0X04, 0X07};
// BYTE COFFEEMILK[] =     {0XD5, 0X55, 0X00, 0X01, 0X03, 0X00, 0X0E, 0X00, 0X08, 0X00, 0X1F, 0X16};
// // BYTE AQUACLEAN[] =      {0XD5, 0X55, 0X00, 0X01, 0X03, 0X00, 0X0E, 0X00, 0X10, 0X00, 0X29, 0X34};
// // BYTE CALCNCLEAN[] =     {0XD5, 0X55, 0X00, 0X01, 0X03, 0X00, 0X0E, 0X00, 0X20, 0X00, 0X0C, 0X35};
// // BYTE STARTPAUSE[] =     {0XD5, 0X55, 0X00, 0X01, 0X03, 0X00, 0X0E, 0X00, 0X00, 0X01, 0X3D, 0X30};
// BYTE REQUESTINFO[] =    {0XD5, 0X55, 0X00, 0X01, 0X03, 0X00, 0X0E, 0X00, 0X00, 0X00, 0X35, 0X34};

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

void PhilipsSeries3200::setup() {
    power_pin_->setup();
    power_pin_->pin_mode(gpio::FLAG_OUTPUT);
    power_pin_->digital_write(true);
}

void PhilipsSeries3200::loop() {
  uint8_t buffer[BUFFER_SIZE];
  uint32_t now = millis();

  if (!this->messages_.empty() && (now - this->last_sent_time_ >= BUTTON_SEQUENCE_DELAY)) {
    uint8_t *message = this->messages_[0];
    this->messages_.erase(this->messages_.begin());
    for (unsigned int i = 0; i <= MESSAGE_REPETITIONS; i++)
      mainboard_uart_->write_array(message, 12);
    this->last_sent_time_ = now;
  }

  // Pipe display to mainboard
  if (display_uart_.available()) {
    uint8_t size = std::min(display_uart_.available(), BUFFER_SIZE);
    display_uart_.read_array(buffer, size);

    mainboard_uart_.write_array(buffer, size);
  }

  // Read until start index
  while (mainboard_uart_.available()) {
    uint8_t buffer = mainboard_uart_.peek();
    if (buffer == 0xD5)
      break;
    display_uart_.write(mainboard_uart_.read());
  }

  // Pipe to display
  if (mainboard_uart_.available()) {
    uint8_t size = std::min(mainboard_uart_.available(), BUFFER_SIZE);
    mainboard_uart_.read_array(buffer, size);

    display_uart_.write_array(buffer, size);

    // Only process messages starting with start bytes
    if (size > 1 && buffer[0] == 0xD5 && buffer[1] == 0x55) {
        last_message_from_mainboard_time_ = millis();

        // Update status sensors
        for (philips_status_sensor::StatusSensor *status_sensor : status_sensors_)
            status_sensor->update_status(buffer, size);
        // Update debug sensors
        for (philips_debug_sensor::DebugSensor *debug_sensor : debug_sensors_)
            debug_sensor->update_debug(buffer, size);
    }
  }

  // Publish power state if required as long as the display is requesting messages
  if (now - last_message_from_mainboard_time_ > POWER_STATE_TIMEOUT) {
    // Update power switches
    for (philips_power_switch::Power *power_switch : power_switches_)
      power_switch->publish_state(false);

    // Update status sensors
    for (philips_status_sensor::StatusSensor *status_sensor : status_sensors_)
      status_sensor->set_state_off();
    // Update debug sensors
    for (philips_debug_sensor::DebugSensor *debug_sensor : debug_sensors_)
      debug_sensor->set_state_off();
  } else {
    // Update power switches
    for (philips_power_switch::Power *power_switch : power_switches_)
      power_switch->publish_state(true);
  }

  display_uart_.flush();
  mainboard_uart_.flush();
}

void PhilipsSeries3200::dump_config() {
  ESP_LOGCONFIG(TAG, "Philips Series 3200");
  LOG_PIN(TAG, "  Power Pin:", this->power_pin_);
  display_uart_.check_uart_settings(115200, 1, uart::UART_CONFIG_PARITY_NONE, 8);
  mainboard_uart_.check_uart_settings(115200, 1, uart::UART_CONFIG_PARITY_NONE, 8);
}

void PhilipsSeries3200::do_action(Action action) {
  switch (action) {
    case SELECT_COFFEE:
    case MAKE_COFFEE:
      this->messages_.push_back(MSG_SELECT_COFFEE);
      if (action == MAKE_COFFEE)
        this->messages_.push_back(MSG_PLAY_PAUSE);
      break;
    case SELECT_ESPRESSO:
    case MAKE_ESPRESSO:
      this->messages_.push_back(MSG_SELECT_ESPRESSO);
      if (action == MAKE_ESPRESSO)
        this->messages_.push_back(MSG_PLAY_PAUSE);
      break;
    case SELECT_AMERICANO:
    case MAKE_AMERICANO:
      this->messages_.push_back(MSG_SELECT_AMERICANO);
      if (action == MAKE_AMERICANO)
        this->messages_.push_back(MSG_PLAY_PAUSE);
      break;
    case SELECT_CAPPUCCINO:
    case MAKE_CAPPUCCINO:
      this->messages_.push_back(MSG_SELECT_CAPPUCCINO);
      if (action == MAKE_CAPPUCCINO)
        this->messages_.push_back(MSG_PLAY_PAUSE);
      break;
    case SELECT_LATTE:
    case MAKE_LATTE:
      this->messages_.push_back(MSG_SELECT_LATTE);
      if (action == MAKE_LATTE)
        this->messages_.push_back(MSG_PLAY_PAUSE);
      break;
    case SELECT_HOT_WATER:
    case MAKE_HOT_WATER:
      this->messages_.push_back(MSG_SELECT_HOT_WATER);
      if (action == MAKE_HOT_WATER)
        this->messages_.push_back(MSG_PLAY_PAUSE);
      break;
    case SELECT_BEAN_AMOUNT:
      this->messages_.push_back(MSG_SELECT_BEAN_AMOUNT);
      break;
    case SELECT_WATER_AMOUNT:
      this->messages_.push_back(MSG_SELECT_WATER_AMOUNT);
      break;
    case SELECT_FROTHED_MILK_AMOUNT:
      this->messages_.push_back(MSG_SELECT_FROTHED_MILK_AMOUNT);
      break;
    case SELECT_AQUA_CLEAN:
      this->messages_.push_back(MSG_SELECT_AQUA_CLEAN);
      break;
    case SELECT_CALC_CLEAN:
      this->messages_.push_back(MSG_SELECT_CALC_CLEAN);
      break;
    case PLAY_PAUSE:
      this->messages_.push_back(MSG_PLAY_PAUSE);
      break;
    default:
      ESP_LOGE(TAG, "Invalid Action %d", action);
  }
}

} // namespace philips_series_3200
} // namespace esphome
