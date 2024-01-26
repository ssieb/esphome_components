import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, key_provider
from esphome.const import CONF_ID

CODEOWNERS = ["@ssieb"]
DEPENDENCIES = ["i2c"]

AUTO_LOAD = [ "key_provider" ]

MULTI_CONF = True

cardkb_ns = cg.esphome_ns.namespace('cardkb')
CardKB = cardkb_ns.class_('CardKB', cg.Component, i2c.I2CDevice, key_provider.KeyProvider)

CONF_CARDKB_ID = 'cardkb_id'

CONFIG_SCHEMA = i2c.i2c_device_schema(None).extend(
    {
        cv.GenerateID(): cv.declare_id(CardKB),
    }
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
