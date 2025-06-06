import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_ID

CODEOWNERS = ["@ssieb"]
MULTI_CONF = True

m5_8encoder_ns = cg.esphome_ns.namespace("m5_8encoder")
M5_8encoder = m5_8encoder_ns.class_("M5_8encoder", i2c.I2CDevice, cg.Component)

CONF_M5_8ENCODER = "m5_8encoder"

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(M5_8encoder),
    }
).extend(i2c.i2c_device_schema(0x41))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

