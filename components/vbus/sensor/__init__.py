import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID
from .. import vbus_ns, VBus, CONF_VBUS_ID

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_VBUS_ID): cv.use_id(VBus),
})

async def to_code(config):
    vbus = await cg.get_variable(config[CONF_VBUS_ID])

