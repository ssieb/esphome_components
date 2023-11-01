import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components.uart import UARTComponent

from esphome.const import CONF_ID

DEPENDENCIES = ['uart']

DISPLAY_UART_ID = 'display_uart'
MAINBOARD_UART_ID = 'mainboard_uart'
POWER_PIN = 'power_pin'
CONF_PHILIPS_ID = 'philips_id'

philips_series_3200_ns = cg.esphome_ns.namespace('philips_series_3200')
PhilipsSeries3200 = philips_series_3200_ns.class_('PhilipsSeries3200', cg.Component)

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(PhilipsSeries3200),
        cv.Required(DISPLAY_UART_ID): cv.use_id(UARTComponent),
        cv.Required(MAINBOARD_UART_ID): cv.use_id(UARTComponent),
        cv.Required(POWER_PIN): pins.gpio_output_pin_schema
    }
)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)

    display = yield cg.get_variable(config[DISPLAY_UART_ID])
    mainboard = yield cg.get_variable(config[MAINBOARD_UART_ID])
    pin = yield cg.gpio_pin_expression(config[POWER_PIN])

    cg.add(var.set_display_uart(display))
    cg.add(var.set_mainboard_uart(mainboard))
    cg.add(var.set_power_pin(pin))
