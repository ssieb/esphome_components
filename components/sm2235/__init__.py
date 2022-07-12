import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_ID, CONF_NUM_CHANNELS

DEPENDENCIES = ["i2c"]
MULTI_CONF = True

sm2235_ns = cg.esphome_ns.namespace("sm2235")
SM2235 = sm2235_ns.class_("SM2235", cg.Component, i2c.I2CDevice)

CONF_RGB_CURRENT = "rgb_current"
CONF_CW_CURRENT = "cw_current"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(SM2235),
            cv.Optional(CONF_RGB_CURRENT, default=12): cv.int_range(min=4, max=64),
            cv.Optional(CONF_CW_CURRENT, default=25): cv.int_range(min=5, max=80),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    currents = round((config[CONF_RGB_CURRENT] - 4) / 4) * 16 + round((config[CONF_CW_CURRENT] - 5) / 5)
    cg.add(var.set_currents(currents))
