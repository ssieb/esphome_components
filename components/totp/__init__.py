import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_TIME_ID
from esphome.components import sensor, text_sensor, time

DEPENDENCIES = ['time']
AUTO_LOAD = ['sensor', 'text_sensor']

MULTI_CONF = True

CONF_SECRET = "secret"
CONF_TOTP = "totp"
CONF_COUNTDOWN = "countdown"

totp_ns = cg.esphome_ns.namespace('totp')
TOTPComponent = totp_ns.class_('TOTPComponent', cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(TOTPComponent),
    cv.Required(CONF_SECRET): cv.string,
    cv.GenerateID(CONF_TIME_ID): cv.use_id(time.RealTimeClock),
    cv.Optional(CONF_TOTP): text_sensor.text_sensor_schema(),
    cv.Optional(CONF_COUNTDOWN): sensor.sensor_schema(),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    cg.add(var.set_secret(config[CONF_SECRET]))
    time_ = await cg.get_variable(config[CONF_TIME_ID])
    cg.add(var.set_time(time_))
    if conf := config.get(CONF_TOTP):
        sens = await text_sensor.new_text_sensor(conf)
        cg.add(var.set_totp(sens))
    if conf := config.get(CONF_COUNTDOWN):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_countdown(sens))
