import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, binary_sensor
from esphome.const import (
    CONF_ID,
)

CODEOWNERS = ["@ssieb"]
MULTI_CONF = True

AUTO_LOAD = [ "binary_sensor" ]
DEPENDENCIES = [ "i2c" ]

mill_ns = cg.esphome_ns.namespace("mill")
Mill = mill_ns.class_("Mill", i2c.I2CDevice, cg.Component)

CONF_MILL_ID = "mill_id"
CONF_PLUS_KEY = "plus_key"
CONF_MINUS_KEY = "minus_key"
CONF_WIFI_KEY = "wifi_key"
CONF_CLOCK_KEY = "clock_key"

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(Mill),
        cv.Optional(CONF_PLUS_KEY): binary_sensor.binary_sensor_schema(),
        cv.Optional(CONF_MINUS_KEY): binary_sensor.binary_sensor_schema(),
        cv.Optional(CONF_WIFI_KEY): binary_sensor.binary_sensor_schema(),
        cv.Optional(CONF_CLOCK_KEY): binary_sensor.binary_sensor_schema(),
    }
).extend(i2c.i2c_device_schema(0x50))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    if CONF_PLUS_KEY in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_PLUS_KEY])
        cg.add(var.set_plus_key(sens))
    if CONF_MINUS_KEY in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_MINUS_KEY])
        cg.add(var.set_minus_key(sens))
    if CONF_WIFI_KEY in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_WIFI_KEY])
        cg.add(var.set_wifi_key(sens))
    if CONF_CLOCK_KEY in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_CLOCK_KEY])
        cg.add(var.set_clock_key(sens))

