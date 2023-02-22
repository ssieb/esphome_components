import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

CODEOWNERS = ["@ssieb"]

DEPENDENCIES = ["uart"]

MULTI_CONF = True

sdi12_ns = cg.esphome_ns.namespace("sdi12")
SDI12 = sdi12_ns.class_("SDI12", uart.UARTDevice, cg.Component)

CONF_SDI12_ID = "sdi12_id"

CONFIG_SCHEMA = uart.UART_DEVICE_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(SDI12),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
