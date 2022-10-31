import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, binary_sensor, sensor
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_CARBON_DIOXIDE,
    ICON_MOLECULE_CO2,
    STATE_CLASS_MEASUREMENT,
    UNIT_PARTS_PER_MILLION,
)

AUTO_LOAD = [ "binary_sensor" ]
DEPENDENCIES = [ "i2c" ]

t67xx_ns = cg.esphome_ns.namespace("t67xx")
T67xx = t67xx_ns.class_("T67xx", sensor.Sensor, cg.PollingComponent, i2c.I2CDevice)

CONF_CALIBRATING = "calibrating"

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        T67xx,
        unit_of_measurement=UNIT_PARTS_PER_MILLION,
        icon=ICON_MOLECULE_CO2,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_CARBON_DIOXIDE,
        state_class=STATE_CLASS_MEASUREMENT,
    )
    .extend(cv.Schema({cv.Optional(CONF_CALIBRATING): binary_sensor.binary_sensor_schema()}))
    .extend(cv.polling_component_schema("60s"))
    .extend(i2c.i2c_device_schema(0x15))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
    await i2c.register_i2c_device(var, config)

    if CONF_CALIBRATING in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_CALIBRATING])
        cg.add(var.set_calibrating(sens))
