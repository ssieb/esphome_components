import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import output
from esphome.const import CONF_CHANNEL, CONF_ID
from . import PCA963xOutput, pca963x_ns

DEPENDENCIES = ['pca963x']

PCA963xChannel = pca963x_ns.class_('PCA963xChannel', output.FloatOutput)
CONF_PCA963X_ID = 'pca963x_id'
CONF_ISOLATE = 'isolate'

CONFIG_SCHEMA = output.FLOAT_OUTPUT_SCHEMA.extend({
    cv.Required(CONF_ID): cv.declare_id(PCA963xChannel),
    cv.GenerateID(CONF_PCA963X_ID): cv.use_id(PCA963xOutput),
    cv.Required(CONF_CHANNEL): cv.int_range(min=0, max=7),
})


async def to_code(config):
    paren = await cg.get_variable(config[CONF_PCA963X_ID])
    rhs = paren.create_channel(config[CONF_CHANNEL])
    var = cg.Pvariable(config[CONF_ID], rhs)
    await output.register_output(var, config)
