#include "esphome.h"
#include "apcups.h"

namespace esphome {
namespace apc {

static const char *TAG = "apcups";

void ApcSmartUpsSerial::dump_config() {
  ESP_LOGCONFIG(TAG, "APC UPS:");
  if (this->in_setup_) {
    if (this->state_ == 0)
      ESP_LOGCONFIG(TAG, "  UPS setup failed");
    else
      ESP_LOGCONFIG(TAG, "  still setting up UPS");
    return;
  }
  ESP_LOGCONFIG(TAG, "  model '%s' fw ver '%s' sernum '%s'",
      this->model_.c_str(), this->firmware_ver_.c_str(), this->serial_num_.c_str());
}

void ApcSmartUpsSerial::setup() {
  this->write_byte('Y');     // send Y char to enable Smart Mode, answer is "SM" if OK
  this->in_setup_ = true;
  this->state_ = 1;
  this->cmd_start_ = millis();
}

void ApcSmartUpsSerial::loop() {
  unsigned long now = millis();
  if (!this->available()) {
     if ((this->state_ > 0) && (millis() - this->cmd_start_ > 500)) {
       ESP_LOGE(TAG, "command timeout");
       if (this->in_setup_)
         ESP_LOGE(TAG, "setup failed");
       this->state_ = 0;
     }
     return;
  }
  while (this->available()) {
    uint8_t c;
    this->read_byte(&c);
    if (this->state_ == 0)
      this->handle_int_(c);
    else if (c == '\r')
      continue;
    else if (c == '\n') {
      if (this->in_setup_)
        this->handle_setup_line_();
      else
        this->handle_line_();
      this->line_.clear();
    } else
      this->line_.push_back(c);
  }
}

void ApcSmartUpsSerial::update() {
  if (this->state_ > 0) {
    ESP_LOGE(TAG, "update interval overrun");
    return;
  }
  this->state_ = 1;
  this->write_byte('Y');
  this->cmd_start_ = millis();
}

void ApcSmartUpsSerial::handle_setup_line_() {
  unsigned long now = millis();
  switch (this->state_) {
   case 1:  // 'Y'
    if (this->line_ != "SM") {
      ESP_LOGE(TAG, "failed to enable smart mode");
      this->state_ = 0;
      return;
    }
    break;
   case 2:  // '^A'
    this->model_ = this->line_;
    ESP_LOGE(TAG, "UPS model '%s'", this->model_.c_str());
    break;
   case 3:  // 'b'
    this->firmware_ver_ = this->line_;
    ESP_LOGE(TAG, "UPS firmware version '%s'", this->firmware_ver_.c_str());
    break;
   case 4:  // 'n'
    this->serial_num_ = this->line_;
    ESP_LOGE(TAG, "UPS serial number '%s'", this->serial_num_.c_str());
    break;
  }
  switch (++this->state_) {
   case 2:
     this->write_byte(1);
     break;
   case 3:
     this->write_byte('b');
     break;
   case 4:
     this->write_byte('n');
     break;
   case 5:
    this->state_ = 0;
    this->in_setup_ = false;
    break;
  }
}

void ApcSmartUpsSerial::handle_line_() {
  unsigned long now = millis();
  float value;
  switch (this->state_) {
   case 1:  // 'Y'
    if (this->line_ != "SM") {
      ESP_LOGE(TAG, "failed to enable smart mode");
      this->state_ = 0;
      this->in_setup_ = true;
      return;
    }
    break;
   case 2:
    value = atof(this->line_.c_str());
    this->internal_temp_sensor_->publish_state(value);
    break;
   case 3:
    value = atof(this->line_.c_str());
    this->battery_volt_sensor_->publish_state(value);
    break;
   case 4:
    value = atof(this->line_.c_str());
    this->battery_level_sensor_->publish_state(value);
    break;
   case 5:
    value = atof(this->line_.c_str());
    this->power_load_sensor_->publish_state(value);
    break;
   case 6:
    value = atof(this->line_.c_str());
    this->input_volt_sensor_->publish_state(value);
    break;
   case 7:
    value = atof(this->line_.c_str());
    this->output_volt_sensor_->publish_state(value);
    break;
   case 8:
    value = atof(this->line_.c_str());
    this->line_freq_sensor_->publish_state(value);
    break;
   case 9:
    this->line_.pop_back();  // remove :
    value = atof(this->line_.c_str());
    this->est_runtime_sensor_->publish_state(value);
    break;
  }
  while (true) {
    switch (++this->state_) {
     case 2:
       if (this->internal_temp_sensor_ != nullptr) {
         this->write_byte('C');
         return;
       }
       break;
     case 3:
       if (this->battery_volt_sensor_ != nullptr) {
         this->write_byte('B');
         return;
       }
       break;
     case 4:
       if (this->battery_level_sensor_ != nullptr) {
         this->write_byte('f');
         return;
       }
       break;
     case 5:
       if (this->power_load_sensor_ != nullptr) {
         this->write_byte('P');
         return;
       }
       break;
     case 6:
       if (this->input_volt_sensor_ != nullptr) {
         this->write_byte('L');
         return;
       }
       break;
     case 7:
       if (this->output_volt_sensor_ != nullptr) {
         this->write_byte('O');
         return;
       }
       break;
     case 8:
       if (this->line_freq_sensor_ != nullptr) {
         this->write_byte('F');
         return;
       }
       break;
     case 9:
       if (this->est_runtime_sensor_ != nullptr) {
         this->write_byte('F');
         return;
       }
       break;
     case 10:
      this->state_ = 0;
      return;
    }
  }
}

void ApcSmartUpsSerial::handle_int_(uint8_t c) {
  switch(c) {
   case '!':
    ESP_LOGW(TAG, "UPS on battery");
    break;
   case '$':
    ESP_LOGW(TAG, "UPS off battery");
    break;
   case '%':
    ESP_LOGW(TAG, "UPS battery is low");
    break;
   case '+':
    ESP_LOGW(TAG, "UPS battery is ok");
    break;
   case '#':
    ESP_LOGW(TAG, "UPS battery needs replacing");
    break;
   default:
    ESP_LOGD(TAG, "received unknown interrupt code: '%c'", c);
  }
}

}  // namespace apc
}  // namespace esphome

