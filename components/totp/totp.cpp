#include "totp.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"
#include <mbedtls/md.h>

namespace esphome {
namespace totp {

static const char *TAG = "totp";

void TOTPComponent::loop() {
  auto now = this->time_->now();
  if (!this->init_) {
    if (!now.is_valid())
      return;
    this->init_ = true;
  } else {
    int current_second = this->next_time_step_ - now.timestamp;
    if (current_second > 0) {
      if (current_second != this->last_second_) {
        this->last_second_ = current_second;
        if (this->countdown_ != nullptr)
          this->countdown_->publish_state(current_second);
      }
      return;
    }
  }
  this->last_totp_ = this->generate_totp_(now);
  if (this->totp_ != nullptr)
    this->totp_->publish_state(this->last_totp_);
  this->next_time_step_ = now.timestamp - (now.timestamp % 30) + 30;
  this->last_second_ = this->next_time_step_ - now.timestamp;
  if (this->countdown_ != nullptr)
    this->countdown_->publish_state(this->last_second_);
}

std::string TOTPComponent::get_current_totp() {
  return this->last_totp_;
}

int TOTPComponent::get_countdown() const {
  return this->last_second_;
}

uint32_t TOTPComponent::get_next_update_time() const {
  return this->next_time_step_;
}

std::string TOTPComponent::generate_totp_(ESPTime &now) {
  uint32_t time_step = now.timestamp / 30;  // Time step of 30 seconds

  unsigned char time_bytes[8];
  for (int i = 7; i >= 0; i--) {
    time_bytes[i] = time_step & 0xFF;
    time_step >>= 8;
  }

  const unsigned char* secret = reinterpret_cast<const unsigned char*>(this->secret_.c_str());
  size_t secret_len = this->secret_.length();

  unsigned char hmac[20];
  mbedtls_md_context_t ctx;
  mbedtls_md_init(&ctx);
  if (mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(MBEDTLS_MD_SHA1), 1) != 0) {
    ESP_LOGE(TAG, "Failed to setup HMAC context");
    return "000000";
  }
  mbedtls_md_hmac_starts(&ctx, secret, secret_len);
  mbedtls_md_hmac_update(&ctx, time_bytes, sizeof(time_bytes));
  mbedtls_md_hmac_finish(&ctx, hmac);
  mbedtls_md_free(&ctx);

  int offset = hmac[19] & 0xF;
  uint32_t code = (hmac[offset] & 0x7F) << 24 |
                  (hmac[offset + 1] & 0xFF) << 16 |
                  (hmac[offset + 2] & 0xFF) << 8 |
                  (hmac[offset + 3] & 0xFF);
  code = code % 1000000;
  return str_sprintf("%06d", code);
}

}  // namespace totp
}  // namespace esphome
