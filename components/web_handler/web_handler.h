#pragma once

#include "esphome/core/component.h"
#include "esphome/core/automation.h"
#include "esphome/components/web_server_base/web_server_base.h"

namespace esphome {
namespace web_handler {

class RequestHandler;

class WebHandler : public Component, public AsyncWebHandler {
 public:
  WebHandler(web_server_base::WebServerBase *base) : base_(base) {}
  void setup() override;
  void dump_config() override;

  void set_path(std::string path) { path_ = path; };
  void set_mime_type(std::string mime_type) { mime_type_ = mime_type; };
  void set_request_handler(RequestHandler *handler) { handler_ = handler; };
  bool canHandle(AsyncWebServerRequest *request) const override;
  void handleRequest(AsyncWebServerRequest *request) override;
  bool isRequestHandlerTrivial() const override;

 protected:
  web_server_base::WebServerBase *base_;
  std::string path_;
  std::string mime_type_;
  RequestHandler *handler_;
};

class RequestHandler : public Trigger<AsyncWebServerRequest &, AsyncResponseStream &> {
 public:
  explicit RequestHandler(WebHandler *parent);
};

}  // namespace web_handler
}  // namespace esphome

