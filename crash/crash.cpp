#include "crash.h"
#include "esphome/core/log.h"

namespace esphome {
namespace crash {

static const char *TAG = "crash";

// the data needs to be touched to ensure that it doesn't get optimized out
void stack_hog(int n, int x) {
  uint8_t buf[128];
  ESP_LOGD(TAG, "hogging the stack level %d", n);
  for (int i = 0; i < 128; i++)
    buf[i] = i;
  int j = 0;
  for (int i = 0; i < 128; i++)
    j += buf[i];
  stack_hog(n + 1, j);
  // need something after to avoid tail recursion optimization
  // will never actually be reached
  ESP_LOGD(TAG, "returning the stack level %d", n);
}

void Crash::loop() {
  stack_hog(0, 0);
}

}  // namespace crash
}  // namespace esphome

