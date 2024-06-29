import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

CODEOWNERS = ["@ssieb"]

DEPENDENCIES = ["uart"]

MULTI_CONF = True

u_inverter_ns = cg.esphome_ns.namespace("u_inverter")
UInverter = u_inverter_ns.class_("UInverter", uart.UARTDevice, cg.PollingComponent)

CONF_U_INVERTER_ID = "u_inverter_id"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(UInverter),
        }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
    .extend(cv.polling_component_schema("60s"))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
