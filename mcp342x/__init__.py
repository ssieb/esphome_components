import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_ID

DEPENDENCIES = ['i2c']
AUTO_LOAD = ['sensor']
MULTI_CONF = True

mcp342x_ns = cg.esphome_ns.namespace('mcp342x')
MCP342XComponent = mcp342x_ns.class_('MCP342XComponent', cg.Component, i2c.I2CDevice)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(MCP342XComponent),
}).extend(cv.COMPONENT_SCHEMA).extend(i2c.i2c_device_schema(0x68))

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield i2c.register_i2c_device(var, config)
