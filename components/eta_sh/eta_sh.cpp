#include "eta_sh.h"
#include "esphome/core/log.h"

namespace esphome {
namespace eta_sh {

static const char *TAG = "eta_sh";

void ETA_SH::setup() {
  std::vector<uint8_t> buffer;
  int count = 0;

  auto add_sensor = [&](sensor::Sensor *sensor, uint16_t datapoint) {
    if (sensor == nullptr)
      return;
    count++;
    buffer.push_back(0x08);
    buffer.push_back(datapoint >> 8);
    buffer.push_back(datapoint & 0xff);
  };

  buffer.push_back('{');
  buffer.push_back('M');
  buffer.push_back('C');
  buffer.push_back(this->update_interval_);
  buffer.push_back(0);
  buffer.push_back(0);
  add_sensor(this->fan_speed_sensor_,7);
  add_sensor(this->boiler_temp_sensor_, 8);
  add_sensor(this->return_temp_sensor_, 9);
  add_sensor(this->buffer_bottom_temp_sensor_, 10);
  add_sensor(this->buffer_middle_temp_sensor_, 11);
  add_sensor(this->buffer_top_temp_sensor_, 12);
  add_sensor(this->exhaust_temp_sensor_, 15);
  add_sensor(this->heater_status_sensor_, 39);
  add_sensor(this->oxygen_sensor_ ,40);
  add_sensor(this->room1_temp_sensor_, 66);
  add_sensor(this->room1_output_temp_sensor_, 68);
  add_sensor(this->outside_temp_sensor_, 70);
  add_sensor(this->buffer_load_sensor_, 75);
  add_sensor(this->external_heater_temp_sensor_, 117);
  if (count == 0) {
    ESP_LOGW(TAG, "no sensors configured");
    return;
  }
  buffer[3] = count * 3 + 1;
  uint8_t csum = 0;
  for (int i = 5; i < buffer.size(); i++)
    csum += buffer[i];
  buffer[4] = csum;
  buffer.push_back('}');
  this->write_array(buffer);
}

void ETA_SH::loop() {
  while (this->available()) {
    uint8_t c;
    this->read_byte(&c);
    if (!this->reading_) {
      if (c == '{') {
        this->reading_ = true;
        this->buffer_.clear();
      }
      continue;
    }
    if ((this->buffer_.size() < 4) || (this->buffer_.size() < this->buffer_[2] + 4)) {
      this->buffer_.push_back(c);
      continue;
    }

    this->reading_ = false;
    if (c != '}') {
      ESP_LOGE(TAG, "didn't find end tag");
      continue;
    }

    uint8_t *data = this->buffer_.data();
    uint8_t csum = 0;
    for (int i = 4; i < this->buffer_.size(); i++)
      csum += data[i];
    if (csum != data[3]) {
      ESP_LOGE(TAG, "checksum failed: %02x != %02x", csum, data[3]);
      continue;
    }
    handle_data_(data);
  }
}

uint16_t inline get16(uint8_t *data) {
  return (data[0] << 8) | data[1];
}

void ETA_SH::handle_data_(uint8_t *data) {
  if ((data[0] != 'M') || (data[1] != 'D')) {
    ESP_LOGV(TAG, "unhandled message: '%c%c'", data[0], data[1]);
    return;
  }
  int count1 = data[2]; 											// set count to num of data packets                         
  data += 4;                                                   
  for (int count = count1; count > 0; count -= 5, data += 5) { 		//  for (int count = data[2]; count > 0; count -= 5, data += 5) { <--wrong, because already moved on, new count1
    if (data[0] != 8) {
      ESP_LOGV(TAG, "data not for us: %02x (%d)", data[0], data[0]);
      continue;
    }
    uint16_t datapoint = get16(data + 1);
    switch (datapoint) {
     case 7:
	  if (this->fan_speed_sensor_ != nullptr)
		  this->fan_speed_sensor_->publish_state((float)get16(data +3));
	  break;
     case 8:
      if (this->boiler_temp_sensor_ != nullptr)
        this->boiler_temp_sensor_->publish_state((float)get16(data + 3) / 10);
      break;
     case 9:
      if (this->return_temp_sensor_ != nullptr)
        this->return_temp_sensor_->publish_state((float)get16(data + 3) / 10);
      break;
     case 10:
      if (this->buffer_bottom_temp_sensor_ != nullptr)
        this->buffer_bottom_temp_sensor_->publish_state((float)get16(data + 3) / 10);
      break;
     case 11:
      if (this->buffer_middle_temp_sensor_ != nullptr)
        this->buffer_middle_temp_sensor_->publish_state((float)get16(data + 3) / 10);
      break;
     case 12:
      if (this->buffer_top_temp_sensor_ != nullptr)
        this->buffer_top_temp_sensor_->publish_state((float)get16(data + 3) / 10);
      break;
     case 15:
      if (this->exhaust_temp_sensor_ != nullptr)
        this->exhaust_temp_sensor_->publish_state((float)get16(data + 3) / 10);
      break;
	 case 39:
	   if (this->heater_status_sensor_ != nullptr)
	   this->heater_status_sensor_->publish_state((float)get16(data + 3));
	  break; 
	 case 40:
      if (this->oxygen_sensor_ != nullptr) {
		  uint16_t val = get16(data + 3);
		  val = (val == 32768 ? 0 :val);
		  this->oxygen_sensor_->publish_state((float)val /100);
	  }
      //  this->oxygen_sensor_->publish_state((float)get16(data + 3) / 100);
	  break;
	 case 66:
	  if (this->room1_temp_sensor_ != nullptr)
		this->room1_temp_sensor_->publish_state((float)get16(data +3) / 10);
	  break;
	 case 68:
	  if (this->room1_output_temp_sensor_ != nullptr)
		this->room1_output_temp_sensor_->publish_state((float)get16(data +3) / 10);
	  break;
     case 70:
      if (this->outside_temp_sensor_ != nullptr)
        this->outside_temp_sensor_->publish_state((float)get16(data + 3) / 10);
      break;
     case 75:
      if (this->buffer_load_sensor_ != nullptr)
        this->buffer_load_sensor_->publish_state(get16(data + 3));
      break;
	 case 117:
	  if (this->external_heater_temp_sensor_ != nullptr)
		this->external_heater_temp_sensor_->publish_state((float)get16(data +3) /10);
	  break;
     default:
      ESP_LOGV(TAG, "unknown data value: %02x (%d)", datapoint, datapoint);
      break;

    }
  }
}

void ETA_SH::dump_config() {
  ESP_LOGCONFIG(TAG, "ETA SH:");
  ESP_LOGCONFIG(TAG, "  Update interval: %ds", this->update_interval_);
  LOG_SENSOR("", "Fan Speed", this->fan_speed_sensor_);
  LOG_SENSOR("", "Boiler Temperature", this->boiler_temp_sensor_);
  LOG_SENSOR("", "Return Temperature", this->return_temp_sensor_);
  LOG_SENSOR("", "Buffer Bottom Temperature", this->buffer_bottom_temp_sensor_);
  LOG_SENSOR("", "Buffer Middle Temperature", this->buffer_middle_temp_sensor_);
  LOG_SENSOR("", "Buffer Top Temperature", this->buffer_top_temp_sensor_);
  LOG_SENSOR("", "Exhaust Temperature", this->exhaust_temp_sensor_);
  LOG_SENSOR("", "Heater Status", this->heater_status_sensor_);
  LOG_SENSOR("", "Oxygen Sensor", this->oxygen_sensor_);
  LOG_SENSOR("", "Room 1 Temperature", this->room1_temp_sensor_);
  LOG_SENSOR("", "Room 1 output Temperature", this->room1_output_temp_sensor_);
  LOG_SENSOR("", "Outside Temperature", this->outside_temp_sensor_);
  LOG_SENSOR("", "Buffer Load", this->buffer_load_sensor_);
  LOG_SENSOR("", "external Heater Temperature", this->external_heater_temp_sensor_);
}

}  // namespace eta_sh
}  // namespace esphome