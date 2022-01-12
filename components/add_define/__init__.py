import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_NAME, CONF_VALUE

MULTI_CONF = True

CONFIG_SCHEMA = cv.Schema({
    cv.Required(CONF_NAME): cv.string,
    cv.Optional(CONF_VALUE): cv.string,
})

def to_code(config):
    if CONF_VALUE in config:
        cg.add_define(config[CONF_NAME], config[CONF_VALUE])
    else:
        cg.add_define(config[CONF_NAME])

