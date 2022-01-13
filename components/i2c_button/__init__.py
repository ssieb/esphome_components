import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import i2c
from esphome.const import (
    CONF_ID,
    CONF_ON_CLICK,
    CONF_ON_DOUBLE_CLICK
)

CODEOWNERS = ["@ssieb"]
MULTI_CONF = True

i2c_button_ns = cg.esphome_ns.namespace("i2c_button")
I2C_Button = i2c_button_ns.class_("I2C_Button", i2c.I2CDevice, cg.Component)

CONF_A = "a"
CONF_B = "b"
CONF_ON_LONG_CLICK = "on_long_click"

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(I2C_Button),
        cv.Optional(CONF_A): cv.Schema(
            {
                cv.Optional(CONF_ON_CLICK): automation.validate_automation(single=True),
                cv.Optional(CONF_ON_DOUBLE_CLICK): automation.validate_automation(single=True),
                cv.Optional(CONF_ON_LONG_CLICK): automation.validate_automation(single=True),
            }
        ),
        cv.Optional(CONF_B): cv.Schema(
            {
                cv.Optional(CONF_ON_CLICK): automation.validate_automation(single=True),
                cv.Optional(CONF_ON_DOUBLE_CLICK): automation.validate_automation(single=True),
                cv.Optional(CONF_ON_LONG_CLICK): automation.validate_automation(single=True),
            }
        ),
    }
).extend(i2c.i2c_device_schema(0x31))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    if CONF_A in config:
        conf = config[CONF_A]
        if CONF_ON_CLICK in conf:
            await automation.build_automation(
                var.get_a_click_trigger(), [], conf[CONF_ON_CLICK]
            )
        if CONF_ON_DOUBLE_CLICK in conf:
            await automation.build_automation(
                var.get_a_double_click_trigger(), [], conf[CONF_ON_DOUBLE_CLICK]
            )
        if CONF_ON_LONG_CLICK in conf:
            await automation.build_automation(
                var.get_a_long_click_trigger(), [], conf[CONF_ON_LONG_CLICK]
            )

    if CONF_B in config:
        conf = config[CONF_B]
        if CONF_ON_CLICK in conf:
            await automation.build_automation(
                var.get_b_click_trigger(), [], conf[CONF_ON_CLICK]
            )
        if CONF_ON_DOUBLE_CLICK in conf:
            await automation.build_automation(
                var.get_b_double_click_trigger(), [], conf[CONF_ON_DOUBLE_CLICK]
            )
        if CONF_ON_LONG_CLICK in conf:
            await automation.build_automation(
                var.get_b_long_click_trigger(), [], conf[CONF_ON_LONG_CLICK]
            )

