#include "heapmon.h"
#include "esphome/core/log.h"

namespace esphome {
namespace debug {

static const char *TAG = "heapmon";

void HeapMonitor::update() {
  uint32_t free_heap = ESP.getFreeHeap();
  ESP_LOGD(TAG, "Free Heap Size: %u bytes", free_heap);
  this->publish_state(free_heap);
}

}  // namespace debug
}  // namespace esphome

