import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import (
    DEVICE_CLASS_TEMPERATURE,
    ICON_THERMOMETER,
    STATE_CLASS_MEASUREMENT,
    UNIT_CELSIUS,
)

DEPENDENCIES = ['uart']

wts01_ns = cg.esphome_ns.namespace('wts01')
WTS01 = wts01_ns.class_('WTS01', sensor.Sensor, uart.UARTDevice, cg.Component)

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        WTS01,
        unit_of_measurement=UNIT_CELSIUS,
        accuracy_decimals=2,
        icon=ICON_THERMOMETER,
        device_class=DEVICE_CLASS_TEMPERATURE,
        state_class=STATE_CLASS_MEASUREMENT,
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(uart.UART_DEVICE_SCHEMA)
)


async def to_code(config):
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
