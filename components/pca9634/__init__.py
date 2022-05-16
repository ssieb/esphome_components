import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_FREQUENCY, CONF_ID, CONF_INVERTED

DEPENDENCIES = ['i2c']
MULTI_CONF = True

pca9634_ns = cg.esphome_ns.namespace('pca9634')

PCA9634GroupMode = mcp23008_ns.enum('PCA9634GroupMode')
PCA9634_GROUP_MODES = {
    'DIM': PCA9634GroupMode.PCA9634_MODE2_DIM,
    'BLINK': PCA9634GroupMode.PCA9634_MODE2_BLINK,
}

PCA9634OutputMode = mcp23008_ns.enum('PCA9634OutputMode')
PCA9634_OUTPUT_MODES = {
    'TOTEM_POLE': PCA9634OutputMode.PCA9634_MODE2_TOTEM_POLE,
    'OPEN_DRAIN': PCA9634OutputMode.PCA9634_MODE2_OPEN_DRAIN,
}

PCA9634Output = pca9634_ns.class_('PCA9345Output', cg.Component, i2c.I2CDevice)

CONF_GROUP_MODE = 'group_mode'
CONF_OUTPUT_MODE = 'output_mode'

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(PCA9634Output),
    cv.Optional(CONF_INVERTED, default=False): cv.boolean,
    cv.Optional(CONF_GROUP_MODE, default="DIM"): cv.enum(PCA9634_GROUP_MODES, upper=True),
    cv.Optional(CONF_OUTPUT_MODE, default="TOTEM_POLE"): cv.enum(PCA9634_OUTPUT_MODES, upper=True),
}).extend(cv.COMPONENT_SCHEMA).extend(i2c.i2c_device_schema(None))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
