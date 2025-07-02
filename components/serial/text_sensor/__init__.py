import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import uart
from esphome.components import text_sensor
from esphome.const import CONF_ID, CONF_STATE

DEPENDENCIES = ['uart']

serial_ns = cg.esphome_ns.namespace('serial')

SerialTextSensor = serial_ns.class_('SerialTextSensor', cg.Component,
                                    text_sensor.TextSensor, uart.UARTDevice)

CONFIG_SCHEMA = text_sensor.text_sensor_schema(SerialTextSensor).extend(uart.UART_DEVICE_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await text_sensor.register_text_sensor(var, config)
    await uart.register_uart_device(var, config)

