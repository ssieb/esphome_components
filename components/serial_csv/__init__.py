import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import sensor, text_sensor, uart
from esphome.const import CONF_ID, CONF_INDEX, CONF_SENSORS, CONF_TEXT_SENSORS

CODEOWNERS = ["@ssieb"]

MULTI_CONF = True

DEPENDENCIES = ['uart']

AUTO_LOAD = ['sensor', 'text_sensor']

serial_ns = cg.esphome_ns.namespace('serial')

SerialCSV = serial_ns.class_('SerialCSV', cg.Component, sensor.Sensor, uart.UARTDevice)


CONFIG_SCHEMA = uart.UART_DEVICE_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(SerialCSV),
        cv.Required(CONF_SENSORS): cv.ensure_list(
            sensor.SENSOR_SCHEMA.extend(
                {
                    cv.Required(CONF_INDEX): cv.positive_int,
                }
            )
        ),
        cv.Required(CONF_TEXT_SENSORS): cv.ensure_list(
            text_sensor.TEXT_SENSOR_SCHEMA.extend(
                {
                    cv.Required(CONF_INDEX): cv.positive_int,
                }
            )
        ),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
    for conf in config[CONF_SENSORS]:
        sens = await sensor.new_sensor(conf)
        index = conf[CONF_INDEX]
        cg.add(var.add_sensor(index, sens))
    for conf in config[CONF_TEXT_SENSORS]:
        sens = await text_sensor.new_text_sensor(conf)
        index = conf[CONF_INDEX]
        cg.add(var.add_text_sensor(index, sens))
