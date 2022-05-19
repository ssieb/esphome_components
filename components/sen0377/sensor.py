import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor, binary_sensor
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_CARBON_DIOXIDE,
    DEVICE_CLASS_CARBON_MONOXIDE,
    DEVICE_CLASS_GAS,
    DEVICE_CLASS_NITROGEN_DIOXIDE,
    ICON_MOLECULE_CO2,
    STATE_CLASS_MEASUREMENT,
    UNIT_PARTS_PER_MILLION,
)

AUTO_LOAD = ["binary_sensor"]
DEPENDENCIES = ["i2c"]

sen0377_ns = cg.esphome_ns.namespace("sen0377")
Sen0377 = sen0377_ns.class_("Sen0377", cg.PollingComponent, i2c.I2CDevice)

CONF_AMMONIA = "ammonia"
CONF_ISOBUTANE = "isobutane"
CONF_CO = "co"
CONF_DETECT = "detect"
CONF_ETHANOL = "ethanol"
CONF_H2S = "h2s"
CONF_HYDROGEN = "hydrogen"
CONF_METHANE = "methane"
CONF_NO1 = "no1"
CONF_NO2 = "no2"
CONF_PROPANE = "propane"
ICON_MOLECULE_CO = "mdi:molecule-co"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(Sen0377),
            cv.Optional(CONF_CO): sensor.sensor_schema(
                unit_of_measurement=UNIT_PARTS_PER_MILLION,
                icon=ICON_MOLECULE_CO,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_CARBON_MONOXIDE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_METHANE): sensor.sensor_schema(
                unit_of_measurement=UNIT_PARTS_PER_MILLION,
                #icon=ICON_,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_GAS,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_ETHANOL): sensor.sensor_schema(
                unit_of_measurement=UNIT_PARTS_PER_MILLION,
                #icon=ICON_,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_GAS,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_HYDROGEN): sensor.sensor_schema(
                unit_of_measurement=UNIT_PARTS_PER_MILLION,
                #icon=ICON_,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_GAS,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_AMMONIA): sensor.sensor_schema(
                unit_of_measurement=UNIT_PARTS_PER_MILLION,
                #icon=ICON_,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_GAS,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_NO2): sensor.sensor_schema(
                unit_of_measurement=UNIT_PARTS_PER_MILLION,
                #icon=ICON_,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_NITROGEN_DIOXIDE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_DETECT): cv.Schema(
                {
                    cv.Optional(CONF_CO): binary_sensor.binary_sensor_schema(),
                    cv.Optional(CONF_METHANE): binary_sensor.binary_sensor_schema(),
                    cv.Optional(CONF_ETHANOL): binary_sensor.binary_sensor_schema(),
                    cv.Optional(CONF_PROPANE): binary_sensor.binary_sensor_schema(),
                    cv.Optional(CONF_ISOBUTANE): binary_sensor.binary_sensor_schema(),
                    cv.Optional(CONF_HYDROGEN): binary_sensor.binary_sensor_schema(),
                    cv.Optional(CONF_H2S): binary_sensor.binary_sensor_schema(),
                    cv.Optional(CONF_AMMONIA): binary_sensor.binary_sensor_schema(),
                    cv.Optional(CONF_NO1): binary_sensor.binary_sensor_schema(),
                    cv.Optional(CONF_NO2): binary_sensor.binary_sensor_schema(),
                }
            ),
        }
    )
    .extend(cv.polling_component_schema("60s"))
    .extend(i2c.i2c_device_schema(0x75))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    if CONF_CO in config:
        sens = await sensor.new_sensor(config[CONF_CO])
        cg.add(var.set_co_sensor(sens))
    if CONF_METHANE in config:
        sens = await sensor.new_sensor(config[CONF_METHANE])
        cg.add(var.set_methane_sensor(sens))
    if CONF_ETHANOL in config:
        sens = await sensor.new_sensor(config[CONF_ETHANOL])
        cg.add(var.set_ethanol_sensor(sens))
    if CONF_HYDROGEN in config:
        sens = await sensor.new_sensor(config[CONF_HYDROGEN])
        cg.add(var.set_hydrogen_sensor(sens))
    if CONF_AMMONIA in config:
        sens = await sensor.new_sensor(config[CONF_AMMONIA])
        cg.add(var.set_ammonia_sensor(sens))
    if CONF_NO2 in config:
        sens = await sensor.new_sensor(config[CONF_NO2])
        cg.add(var.set_no2_sensor(sens))
    if CONF_DETECT in config:
        conf = config[CONF_DETECT]
        if CONF_CO in conf:
            sens = await binary_sensor.new_binary_sensor(conf[CONF_CO])
            cg.add(var.set_co_bsensor(sens))
        if CONF_METHANE in conf:
            sens = await binary_sensor.new_binary_sensor(conf[CONF_METHANE])
            cg.add(var.set_methane_bsensor(sens))
        if CONF_ETHANOL in conf:
            sens = await binary_sensor.new_binary_sensor(conf[CONF_ETHANOL])
            cg.add(var.set_ethanol_bsensor(sens))
        if CONF_PROPANE in conf:
            sens = await binary_sensor.new_binary_sensor(conf[CONF_PROPANE])
            cg.add(var.set_propane_bsensor(sens))
        if CONF_ISOBUTANE in conf:
            sens = await binary_sensor.new_binary_sensor(conf[CONF_ISOBUTANE])
            cg.add(var.set_isobutane_bsensor(sens))
        if CONF_HYDROGEN in conf:
            sens = await binary_sensor.new_binary_sensor(conf[CONF_HYDROGEN])
            cg.add(var.set_hydrogen_bsensor(sens))
        if CONF_H2S in conf:
            sens = await binary_sensor.new_binary_sensor(conf[CONF_H2S])
            cg.add(var.set_h2s_bsensor(sens))
        if CONF_AMMONIA in conf:
            sens = await binary_sensor.new_binary_sensor(conf[CONF_AMMONIA])
            cg.add(var.set_ammonia_bsensor(sens))
        if CONF_NO1 in conf:
            sens = await binary_sensor.new_binary_sensor(conf[CONF_NO1])
            cg.add(var.set_no_bsensor(sens))
        if CONF_NO2 in conf:
            sens = await binary_sensor.new_binary_sensor(conf[CONF_NO2])
            cg.add(var.set_no2_bsensor(sens))

