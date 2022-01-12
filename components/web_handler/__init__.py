import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import web_server_base
from esphome.components.web_server_base import WebServerBase, CONF_WEB_SERVER_BASE_ID
from esphome.const import CONF_ID, CONF_TRIGGER_ID

AUTO_LOAD = ['web_server']
MULTI_CONF = True

CONF_PATH = "path"
CONF_ON_REQUEST = "on_request"
CONF_MIME_TYPE = "mime_type"

web_handler_ns = cg.esphome_ns.namespace('web_handler')
WebHandler = web_handler_ns.class_('WebHandler', cg.Component)
AsyncWebServerRequest = cg.global_ns.class_('AsyncWebServerRequest')
AsyncWebServerRequestRef = AsyncWebServerRequest.operator('ref')
AsyncResponseStream = cg.global_ns.class_('AsyncResponseStream')
AsyncResponseStreamRef = AsyncResponseStream.operator('ref')
RequestHandler = web_handler_ns.class_('RequestHandler',
        automation.Trigger.template(AsyncWebServerRequestRef, AsyncResponseStreamRef))


CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(WebHandler),
    cv.Required(CONF_PATH): cv.string,
    cv.Optional(CONF_MIME_TYPE, default="text/html"): cv.string,
    cv.Required(CONF_ON_REQUEST): automation.validate_automation({
        cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(RequestHandler),
    }),
    cv.GenerateID(CONF_WEB_SERVER_BASE_ID): cv.use_id(WebServerBase),
})

async def to_code(config):
    paren = await cg.get_variable(config[CONF_WEB_SERVER_BASE_ID])
    var = cg.new_Pvariable(config[CONF_ID], paren)
    cg.add(var.set_path(config[CONF_PATH]))
    cg.add(var.set_mime_type(config[CONF_MIME_TYPE]))
    await cg.register_component(var, config)
    conf = config[CONF_ON_REQUEST][0]
    trig = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
    await automation.build_automation(trig, [(AsyncWebServerRequestRef, 'request'), (AsyncResponseStreamRef, 'stream')], conf)

