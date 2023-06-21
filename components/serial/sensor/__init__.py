import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import uart
from esphome.components import sensor
from esphome.const import CONF_ID

DEPENDENCIES = ['uart']

serial_ns = cg.esphome_ns.namespace('serial')

SerialSensor = serial_ns.class_('SerialSensor', cg.Component, sensor.Sensor, uart.UARTDevice)

CONFIG_SCHEMA = sensor.sensor_schema(SerialSensor).extend(uart.UART_DEVICE_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
    await uart.register_uart_device(var, config)

