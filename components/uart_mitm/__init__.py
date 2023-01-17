import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

DEPENDENCIES = ['uart']

serial_ns = cg.esphome_ns.namespace('serial')

UARTMITM = serial_ns.class_('UARTMITM', cg.Component)

CONF_UART1 = "uart1"
CONF_UART2 = "uart2"

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(UARTMITM),
    cv.Required(CONF_UART1): cv.use_id(uart.UARTComponent),
    cv.Required(CONF_UART2): cv.use_id(uart.UARTComponent),
})


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    uart1 = await cg.get_variable(config[CONF_UART1])
    cg.add(var.set_uart1(uart1))
    uart2 = await cg.get_variable(config[CONF_UART2])
    cg.add(var.set_uart2(uart2))
