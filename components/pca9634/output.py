import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import output
from esphome.const import CONF_CHANNEL, CONF_ID
from . import PCA9634Output, pca9634_ns

DEPENDENCIES = ['pca9634']

PCA9634Channel = pca9634_ns.class_('PCA9634Channel', output.FloatOutput)
CONF_PCA9634_ID = 'pca9634_id'
CONF_ISOLATE = 'isolate'

CONFIG_SCHEMA = output.FLOAT_OUTPUT_SCHEMA.extend({
    cv.Required(CONF_ID): cv.declare_id(PCA9634Channel),
    cv.GenerateID(CONF_PCA9634_ID): cv.use_id(PCA9634Output),
    cv.Required(CONF_CHANNEL): cv.int_range(min=0, max=9),
    cv.Optional(CONF_ISOLATE, default=False): cv.boolean,
})


async def to_code(config):
    paren = await cg.get_variable(config[CONF_PCA9634_ID])
    rhs = paren.create_channel(config[CONF_CHANNEL])
    var = cg.Pvariable(config[CONF_ID], rhs)
    await output.register_output(var, config)
