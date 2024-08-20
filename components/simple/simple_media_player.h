#pragma once

#include "esphome/components/media_player/media_player.h"
#include "esphome/core/component.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace simple {

class SimpleMedia : public Component, public media_player::MediaPlayer {
 public:
  void setup() override;
  float get_setup_priority() const override { return esphome::setup_priority::LATE; }

  void loop() override;

  void dump_config() override;

  media_player::MediaPlayerTraits get_traits() override;

  bool is_muted() const override { return this->muted_; }

 protected:
  void control(const media_player::MediaPlayerCall &call) override;
  bool muted_{false};
};

}  // namespace simple
}  // namespace esphome
