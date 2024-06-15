import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor, i2c, sensor
from esphome.const import (
    CONF_BATTERY_LEVEL,
    CONF_ID,
    DEVICE_CLASS_BATTERY,
    ICON_BATTERY,
    ICON_FLASH,
    STATE_CLASS_MEASUREMENT,
    UNIT_PERCENT,
)

DEPENDENCIES = ["i2c"]
AUTO_LOAD = ["binary_sensor"]

axp2101_ns = cg.esphome_ns.namespace("axp2101")
AXP2101 = axp2101_ns.class_(
    "AXP2101", cg.PollingComponent, i2c.I2CDevice
)

CONF_CHARGING = "charging"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(AXP2101),
            cv.Optional(CONF_BATTERY_LEVEL): sensor.sensor_schema(
                unit_of_measurement=UNIT_PERCENT,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_BATTERY,
                state_class=STATE_CLASS_MEASUREMENT,
                icon=ICON_BATTERY,
            ),
            cv.Optional(CONF_CHARGING): binary_sensor.binary_sensor_schema(
                icon=ICON_FLASH,
            ),
        }
    )
    .extend(cv.polling_component_schema("60s"))
    .extend(i2c.i2c_device_schema(0x34))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    if conf := config.get(CONF_BATTERY_LEVEL):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_battery_level(sens))
    if conf := config.get(CONF_CHARGING):
        bsens = await binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_charging(bsens))
