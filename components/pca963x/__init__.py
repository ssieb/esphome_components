import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_FREQUENCY, CONF_ID, CONF_INVERTED, CONF_MODEL

DEPENDENCIES = ['i2c']
MULTI_CONF = True

pca963X_ns = cg.esphome_ns.namespace('pca9634')

PCA9634GroupMode = pca963X_ns.enum('PCA9634GroupMode')
PCA9634_BLINKING_MODES = {
    'DIM': False,
    'BLINK': True,
}

PCA9634OutputMode = pca963X_ns.enum('PCA9634OutputMode')
PCA9634_OUTPUT_MODES = {
    'TOTEM_POLE': False,
    'OPEN_DRAIN': True,
}

PCA963xOutput = pca963X_ns.class_('PCA963x', cg.Component, i2c.I2CDevice)

CONF_BLINKING_MODE = 'blinking_mode'
CONF_OUTPUT_MODE = 'output_mode'

MODELS = {
    "PCA9633": pca963X_ns.class_("PCA9633Output", PCA963xOutput),
    "PCA9634": pca963X_ns.class_("PCA9634Output", PCA963xOutput),
    "PCA9635": pca963X_ns.class_("PCA9635Output", PCA963xOutput),
}

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(PCA963xOutput),
    cv.Optional(CONF_MODEL, default="PCA9634"): cv.enum(MODELS, upper=True, space="_"),
    cv.Optional(CONF_INVERTED, default=False): cv.boolean,
    cv.Optional(CONF_BLINKING_MODE, default="DIM"): cv.enum(PCA9634_BLINKING_MODES, upper=True),
    cv.Optional(CONF_OUTPUT_MODE, default="TOTEM_POLE"): cv.enum(PCA9634_OUTPUT_MODES, upper=True),
}).extend(cv.COMPONENT_SCHEMA).extend(i2c.i2c_device_schema(0xc0))


async def to_code(config):
    rhs = MODELS[config[CONF_MODEL]].new()
    var = cg.Pvariable(config[CONF_ID], rhs)
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    cg.add(var.set_blinking_mode(config[CONF_BLINKING_MODE]))
    cg.add(var.set_output_mode(config[CONF_OUTPUT_MODE]))
    cg.add(var.set_inverted(config[CONF_INVERTED]))