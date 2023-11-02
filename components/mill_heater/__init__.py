import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import (
    CONF_ID,
)

CODEOWNERS = ["@ssieb"]
MULTI_CONF = True

mill_ns = cg.esphome_ns.namespace("mill")
Mill = mill_ns.class_("Mill", i2c.I2CDevice, cg.Component)

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(Mill),
    }
).extend(i2c.i2c_device_schema(0x50))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

