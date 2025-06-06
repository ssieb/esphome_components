#include "simple_media_player.h"

#include "esphome/core/log.h"

namespace esphome {
namespace simple {

static const char *const TAG = "simple.media";

void SimpleMedia::control(const media_player::MediaPlayerCall &call) {
  if (call.get_announcement().has_value() && call.get_announcement().value()) {
    ESP_LOGE(TAG, "Announcements aren't supported!");
    return;
  }
  if (call.get_media_url().has_value()) {
    ESP_LOGD(TAG, "New URL: '%s'!", call.get_media_url().value().c_str());
  }

  if (call.get_volume().has_value()) {
    this->volume = call.get_volume().value();
    ESP_LOGD(TAG, "Volume %f!", this->volume);
  }
  if (call.get_command().has_value()) {
    switch (call.get_command().value()) {
      case media_player::MEDIA_PLAYER_COMMAND_MUTE:
        ESP_LOGD(TAG, "Mute!");
        this->muted_ = true;
        break;
      case media_player::MEDIA_PLAYER_COMMAND_UNMUTE:
        ESP_LOGD(TAG, "Unmute!");
        this->muted_ = false;
        break;
      case media_player::MEDIA_PLAYER_COMMAND_VOLUME_UP:
        this->volume = std::min(this->volume + 0.1f, 1.0f);
        ESP_LOGD(TAG, "Volume up %f!", this->volume);
        this->muted_ = false;
        break;
      case media_player::MEDIA_PLAYER_COMMAND_VOLUME_DOWN:
        this->volume = std::max(this->volume - 0.1f, 0.0f);
        ESP_LOGD(TAG, "Volume down %f!", this->volume);
        this->muted_ = false;
        break;
      case media_player::MEDIA_PLAYER_COMMAND_PLAY:
        ESP_LOGD(TAG, "Play!");
        this->state = media_player::MEDIA_PLAYER_STATE_PLAYING;
        break;
      case media_player::MEDIA_PLAYER_COMMAND_PAUSE:
        ESP_LOGD(TAG, "Pause!");
        this->state = media_player::MEDIA_PLAYER_STATE_PAUSED;
        break;
      case media_player::MEDIA_PLAYER_COMMAND_STOP:
        ESP_LOGD(TAG, "Stop!");
        this->state = media_player::MEDIA_PLAYER_STATE_IDLE;
        break;
      case media_player::MEDIA_PLAYER_COMMAND_TOGGLE:
        ESP_LOGD(TAG, "Toggle!");
        if (this->state == media_player::MEDIA_PLAYER_STATE_PLAYING) {
          this->state = media_player::MEDIA_PLAYER_STATE_PAUSED;
        } else {
          this->state = media_player::MEDIA_PLAYER_STATE_PLAYING;
        }
        break;
      default:
        ESP_LOGE(TAG, "Unknown command %d!", call.get_command().value());
        break;
    }
  }
  this->publish_state();
}

void SimpleMedia::setup() {
  this->state = media_player::MEDIA_PLAYER_STATE_IDLE;
  this->publish_state();
}

void SimpleMedia::loop() {
}

media_player::MediaPlayerTraits SimpleMedia::get_traits() {
  auto traits = media_player::MediaPlayerTraits();
  traits.set_supports_pause(true);
  return traits;
};

void SimpleMedia::dump_config() {
  ESP_LOGCONFIG(TAG, "Simple Media:");
}

}  // namespace simple
}  // namespace esphome
