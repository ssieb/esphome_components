#pragma once

#include "esphome/components/key_provider/key_provider.h"
#include "esphome/core/automation.h"
#include "esphome/core/log.h"

namespace esphome {
namespace input_builder {



class InputBuilder : public Component {
 public:
  InputBuilder();
  void loop() override;
  void dump_config() override;
  void set_provider(key_provider::KeyProvider *provider);
  void set_min_length(int min_length) { this->min_length_ = min_length; };
  void set_max_length(int max_length) { this->max_length_ = max_length; };
  void set_start_keys(std::string keys) { this->start_keys_ = keys; };
  void set_end_keys(std::string end_keys) { this->end_keys_ = end_keys; };
  void set_end_key_required(bool end_key_required) { this->end_key_required_ = end_key_required; };
  void set_back_keys(std::string back_keys) { this->back_keys_ = back_keys; };
  void set_clear_keys(std::string clear_keys) { this->clear_keys_ = clear_keys; };
  void set_allowed_keys(std::string allowed_keys) { this->allowed_keys_ = allowed_keys; };
  Trigger<std::string> *get_progress_trigger() const { return this->progress_trigger_; };
  Trigger<std::string> *get_result_trigger() const { return this->result_trigger_; };
  void set_timeout(int timeout) { this->timeout_ = timeout; };
  bool is_started() { return this->is_started_; }

 protected:
  void key_pressed_(uint8_t key);
  bool can_handle_(uint8_t key);
  void clear_result_();
 
  int min_length_{0};
  int max_length_{0};
  std::string start_keys_;
  std::string end_keys_;
  bool is_started_{false};
  bool end_key_required_{false};
  std::string back_keys_;
  std::string clear_keys_;
  std::string allowed_keys_;
  std::string result_;
  Trigger<std::string> *progress_trigger_;
  Trigger<std::string> *result_trigger_;
  uint32_t last_key_time_;
  uint32_t timeout_{0};
};


template<typename... Ts> class InputBuilderCondition : public Condition<Ts...> {
 public:
  InputBuilderCondition(InputBuilder *parent, bool state) : parent_(parent), state_(state) {}
  bool check(Ts... x) override { return this->parent_->is_started_ == this->state_; }

 protected:
  InputBuilder *parent_;
  bool state_;
};


}  // namespace input_builder
}  // namespace esphome
