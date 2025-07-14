import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import sensor
from esphome.const import CONF_ID, DEVICE_CLASS_VOLTAGE, ICON_FLASH, UNIT_VOLT
from .. import uart_demo_ns, UARTDemo, CONF_UART_DEMO_ID

UARTDemoSensor = uart_demo_ns.class_("UARTDemoSensor", sensor.Sensor)

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        icon=ICON_FLASH,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_VOLTAGE
    )
    .extend(
        {
            cv.GenerateID(CONF_UART_DEMO_ID): cv.use_id(UARTDemo),
        }
    )
)

async def to_code(config):
    parent = await cg.get_variable(config[CONF_UART_DEMO_ID])
    var = await sensor.new_sensor(config)
    cg.add(var.set_parent(parent))
