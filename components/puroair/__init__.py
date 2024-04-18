import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

DEPENDENCIES = ['uart']

puroair_ns = cg.esphome_ns.namespace('puroair')
PuroAir = puroair_ns.class_('PuroAir', cg.Component)

CONF_BUTTON_UART = "button_uart"
CONF_CONTROL_UART = "control_uart"

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(PuroAir),
    cv.Optional(CONF_BUTTON_UART): cv.use_id(uart.UARTComponent),
    cv.Optional(CONF_CONTROL_UART): cv.use_id(uart.UARTComponent),
})


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    if CONF_BUTTON_UART in config:
        button_uart = await cg.get_variable(config[CONF_BUTTON_UART])
        cg.add(var.set_button_uart(button_uart))
    if CONF_CONTROL_UART in config:
        control_uart = await cg.get_variable(config[CONF_CONTROL_UART])
        cg.add(var.set_control_uart(control_uart))
