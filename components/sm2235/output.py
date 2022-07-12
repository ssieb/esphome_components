import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import output
from esphome.const import CONF_CHANNEL, CONF_ID
from . import SM2235, sm2235_ns

DEPENDENCIES = ["sm2235"]

SM2235Channel = sm2235_ns.class_("SM2235Channel", output.FloatOutput)
CONF_SM2235_ID = "sm2235_id"

CONFIG_SCHEMA = output.FLOAT_OUTPUT_SCHEMA.extend(
    {
        cv.Required(CONF_ID): cv.declare_id(SM2235Channel),
        cv.GenerateID(CONF_SM2235_ID): cv.use_id(SM2235),
        cv.Required(CONF_CHANNEL): cv.int_range(min=0, max=4),
    }
)


async def to_code(config):
    paren = await cg.get_variable(config[CONF_SM2235_ID])
    var = cg.new_Pvariable(config[CONF_ID])
    cg.add(var.set_channel(config[CONF_CHANNEL]))
    cg.add(paren.register_channel(var, config[CONF_CHANNEL]))
    await output.register_output(var, config)
