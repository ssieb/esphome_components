import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins, automation
from esphome.components import uart
from esphome.components import text_sensor
from esphome.const import CONF_ID, CONF_STATE

wiegand_ns = cg.esphome_ns.namespace('wiegand')

WiegandTextSensor = wiegand_ns.class_('WiegandTextSensor', cg.Component, text_sensor.TextSensor)

CONF_D0 = "d0"
CONF_D1 = "d1"

CONFIG_SCHEMA = text_sensor.TEXT_SENSOR_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(WiegandTextSensor),
    cv.Required(CONF_D0): pins.internal_gpio_input_pin_schema,
    cv.Required(CONF_D1): pins.internal_gpio_input_pin_schema,
})


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await text_sensor.register_text_sensor(var, config)
    pin = await cg.gpio_pin_expression(config[CONF_D0])
    cg.add(var.set_d0_pin(pin))
    pin = await cg.gpio_pin_expression(config[CONF_D1])
    cg.add(var.set_d1_pin(pin))

