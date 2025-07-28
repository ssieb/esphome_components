#include "web_handler.h"
#include "esphome/core/log.h"
#include <esphome/components/logger/logger.h>

namespace esphome {
namespace web_handler {

static const char *TAG = "web_handler";

void WebHandler::setup() {
  base_->add_handler(this);
}

void WebHandler::dump_config() {
  ESP_LOGCONFIG(TAG, "Listening on path '%s'", path_.c_str());
}

bool WebHandler::canHandle(AsyncWebServerRequest *request) const {
  return (std::string(request->url().c_str()) == path_);
}

void WebHandler::handleRequest(AsyncWebServerRequest *request) {
  AsyncResponseStream *stream = request->beginResponseStream(mime_type_.c_str());
  handler_->trigger(*request, *stream);
  request->send(stream);
}

bool WebHandler::isRequestHandlerTrivial() const { return false; }

RequestHandler::RequestHandler(WebHandler *parent) {
  parent->set_request_handler(this);
}

}  // namespace web_handler
}  // namespace esphome
