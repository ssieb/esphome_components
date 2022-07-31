import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.const import CONF_ID, CONF_NUM_CHANNELS, CONF_SCL, CONF_SDA

MULTI_CONF = True

sm2235_ns = cg.esphome_ns.namespace("sm2235")
SM2235 = sm2235_ns.class_("SM2235", cg.Component)

CONF_RGB_CURRENT = "rgb_current"
CONF_CW_CURRENT = "cw_current"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(SM2235),
            cv.Required(CONF_SDA): pins.internal_gpio_input_pin_schema,
            cv.Required(CONF_SCL): pins.internal_gpio_input_pin_schema,
            cv.Optional(CONF_RGB_CURRENT, default=12): cv.int_range(min=4, max=64),
            cv.Optional(CONF_CW_CURRENT, default=25): cv.int_range(min=5, max=80),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    pin = await cg.gpio_pin_expression(config[CONF_SDA])
    cg.add(var.set_sda_pin(pin))
    pin = await cg.gpio_pin_expression(config[CONF_SCL])
    cg.add(var.set_scl_pin(pin))
    currents = round((config[CONF_RGB_CURRENT] - 4) / 4) * 16 + round((config[CONF_CW_CURRENT] - 5) / 5)
    cg.add(var.set_currents(currents))
