import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import key_provider, uart
from esphome.const import CONF_ID

AUTO_LOAD = [ "key_provider" ]
DEPENDENCIES = ['uart']

MULTI_CONF = True

honeywell_ns = cg.esphome_ns.namespace('honeywell')

HoneyWell = honeywell_ns.class_('HoneyWell', cg.Component,
                                key_provider.KeyProvider, uart.UARTDevice)

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(HoneyWell),
}).extend(uart.UART_DEVICE_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

