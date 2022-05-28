import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import uart
from esphome.components import sensor
from esphome.const import CONF_ID, CONF_HEIGHT, CONF_TIMEOUT, ICON_GAUGE

DEPENDENCIES = ['uart']
AUTO_LOAD = ['sensor']

desky_ns = cg.esphome_ns.namespace('desky')

Desky = desky_ns.class_('Desky', cg.Component, uart.UARTDevice)

CONF_UP = "up"
CONF_DOWN = "down"
CONF_REQUEST = "request"
CONF_STOPPING_DISTANCE = "stopping_distance"

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(Desky),
    cv.Optional(CONF_UP): pins.gpio_output_pin_schema,
    cv.Optional(CONF_DOWN): pins.gpio_output_pin_schema,
    cv.Optional(CONF_REQUEST): pins.gpio_output_pin_schema,
    cv.Optional(CONF_HEIGHT): sensor.sensor_schema(icon=ICON_GAUGE, accuracy_decimals=0),
    cv.Optional(CONF_STOPPING_DISTANCE, default=15): cv.positive_int,
    cv.Optional(CONF_TIMEOUT): cv.time_period,
}).extend(uart.UART_DEVICE_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
    if CONF_UP in config:
        pin = await cg.gpio_pin_expression(config[CONF_UP])
        cg.add(var.set_up_pin(pin))
    if CONF_DOWN in config:
        pin = await cg.gpio_pin_expression(config[CONF_DOWN])
        cg.add(var.set_down_pin(pin))
    if CONF_REQUEST in config:
        pin = await cg.gpio_pin_expression(config[CONF_REQUEST])
        cg.add(var.set_request_pin(pin))
    if CONF_HEIGHT in config:
        sens = await sensor.new_sensor(config[CONF_HEIGHT])
        cg.add(var.set_height_sensor(sens))
    cg.add(var.set_stopping_distance(config[CONF_STOPPING_DISTANCE]))
    if CONF_TIMEOUT in config:
        cg.add(var.set_timeout(config[CONF_TIMEOUT].total_milliseconds))

