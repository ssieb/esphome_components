import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import uart
from esphome.components import sensor
from esphome.const import CONF_ID, CONF_HEIGHT, CONF_TIMEOUT, ICON_GAUGE, STATE_CLASS_MEASUREMENT

DEPENDENCIES = ['uart']
AUTO_LOAD = ['sensor']

desky_ns = cg.esphome_ns.namespace('desky')

Desky = desky_ns.class_('Desky', cg.Component, uart.UARTDevice)

CONF_UP_PIN = "up_pin"
CONF_UP_IN_PIN = "up_in_pin"
CONF_DOWN_PIN = "down_pin"
CONF_DOWN_IN_PIN = "down_in_pin"
CONF_REQUEST_PIN = "request_pin"
CONF_INTERCEPT = "intercept"
CONF_STOPPING_DISTANCE = "stopping_distance"

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(Desky),
    cv.Optional(CONF_UP_PIN): pins.gpio_output_pin_schema,
    cv.Optional(CONF_DOWN_PIN): pins.gpio_output_pin_schema,
    cv.Optional(CONF_REQUEST_PIN): pins.gpio_output_pin_schema,
    cv.Optional(CONF_UP_IN_PIN): pins.gpio_input_pin_schema,
    cv.Optional(CONF_DOWN_IN_PIN): pins.gpio_input_pin_schema,
    cv.Optional(CONF_HEIGHT): sensor.sensor_schema(
        icon=ICON_GAUGE,
        accuracy_decimals=0,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    cv.Optional(CONF_STOPPING_DISTANCE, default=15): cv.positive_int,
    cv.Optional(CONF_TIMEOUT): cv.time_period,
    cv.Optional(CONF_INTERCEPT, default="false"): cv.boolean,
}).extend(uart.UART_DEVICE_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
    if CONF_UP_PIN in config:
        pin = await cg.gpio_pin_expression(config[CONF_UP_PIN])
        cg.add(var.set_up_pin(pin))
    if CONF_DOWN_PIN in config:
        pin = await cg.gpio_pin_expression(config[CONF_DOWN_PIN])
        cg.add(var.set_down_pin(pin))
    if CONF_REQUEST_PIN in config:
        pin = await cg.gpio_pin_expression(config[CONF_REQUEST_PIN])
        cg.add(var.set_request_pin(pin))
    if CONF_UP_IN_PIN in config:
        pin = await cg.gpio_pin_expression(config[CONF_UP_IN_PIN])
        cg.add(var.set_up_in_pin(pin))
    if CONF_DOWN_IN_PIN in config:
        pin = await cg.gpio_pin_expression(config[CONF_DOWN_IN_PIN])
        cg.add(var.set_down_in_pin(pin))
    if CONF_HEIGHT in config:
        sens = await sensor.new_sensor(config[CONF_HEIGHT])
        cg.add(var.set_height_sensor(sens))
    cg.add(var.set_stopping_distance(config[CONF_STOPPING_DISTANCE]))
    if CONF_TIMEOUT in config:
        cg.add(var.set_timeout(config[CONF_TIMEOUT].total_milliseconds))
    cg.add(var.set_intercept(config[CONF_INTERCEPT]))

