import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, binary_sensor, climate
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
CONF_CLIMATE_ID = "climate_id"
CONF_DARK_TIMEOUT = "dark_timeout"
CONF_KEY_REPEAT_DELAY = "key_repeat_delay"
CONF_KEY_REPEAT_INTERVAL = "key_repeat_interval"
CONF_SET_TEMP_DELAY = "set_temp_delay"
CONF_WIFI_KEY = "wifi_key"
CONF_CLOCK_KEY = "clock_key"

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(Mill),
        cv.GenerateID(CONF_CLIMATE_ID): cv.use_id(climate.Climate),
        cv.Optional(CONF_DARK_TIMEOUT, "10000ms"): cv.positive_time_period_milliseconds,
        cv.Optional(CONF_KEY_REPEAT_DELAY, "500ms"): cv.positive_time_period_milliseconds,
        cv.Optional(CONF_KEY_REPEAT_INTERVAL, "100ms"): cv.positive_time_period_milliseconds,
        cv.Optional(CONF_SET_TEMP_DELAY, "2000ms"): cv.positive_time_period_milliseconds,
        cv.Optional(CONF_WIFI_KEY): binary_sensor.binary_sensor_schema(),
        cv.Optional(CONF_CLOCK_KEY): binary_sensor.binary_sensor_schema(),
    }
).extend(i2c.i2c_device_schema(0x50))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
    clim = await cg.get_variable(config[CONF_CLIMATE_ID])
    cg.add(var.set_climate(clim))
    cg.add(var.set_dark_timeout(config[CONF_DARK_TIMEOUT]))
    cg.add(var.set_key_repeat_delay(config[CONF_KEY_REPEAT_DELAY]))
    cg.add(var.set_key_repeat_interval(config[CONF_KEY_REPEAT_INTERVAL]))
    cg.add(var.set_set_temp_delay(config[CONF_SET_TEMP_DELAY]))

    if CONF_WIFI_KEY in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_WIFI_KEY])
        cg.add(var.set_wifi_key(sens))
    if CONF_CLOCK_KEY in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_CLOCK_KEY])
        cg.add(var.set_clock_key(sens))

