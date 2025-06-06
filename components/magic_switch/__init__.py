import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation, pins
from esphome.const import (
    CONF_ID,
    CONF_PIN,
    CONF_NUMBER,
    CONF_TIMEOUT,
)
from esphome.core import CORE

CONF_MULTI = True

CODEOWNERS = ["@ssieb"]

magic_switch_ns = cg.esphome_ns.namespace("magic_switch")

MagicSwitch = magic_switch_ns.class_("MagicSwitch", cg.Component)

CONF_ON_SWITCH = "on_switch"

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(MagicSwitch),
        cv.Required(CONF_PIN): pins.internal_gpio_input_pin_schema,
        cv.Optional(CONF_ON_SWITCH): automation.validate_automation(single=True),
        cv.Optional(CONF_TIMEOUT, default="12ms"): cv.positive_time_period_microseconds,
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    pin = await cg.gpio_pin_expression(config[CONF_PIN])
    cg.add(var.set_pin(pin))
    cg.add(var.set_timeout(config[CONF_TIMEOUT]))
    if conf := config.get(CONF_ON_SWITCH):
        await automation.build_automation(var.get_switch_trigger(), [], conf)
