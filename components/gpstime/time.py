from esphome.components import time as time_
import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.components import uart
from esphome.const import CONF_ID

CODEOWNERS = ["@ssieb"]

DEPENDENCIES = ['uart']

gpstime_ns = cg.esphome_ns.namespace('gpstime')

GPSTime = gpstime_ns.class_('GPSTime', time_.RealTimeClock, uart.UARTDevice)

CONFIG_SCHEMA = (
    time_.TIME_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(GPSTime),
        }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
    .extend(cv.polling_component_schema("15min"))
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await time_.register_time(var, config)
    await uart.register_uart_device(var, config)

