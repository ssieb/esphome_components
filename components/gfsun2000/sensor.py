import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, text_sensor, modbus
from esphome.const import CONF_ID, UNIT_VOLT, ICON_FLASH, UNIT_AMPERE, UNIT_WATT, UNIT_WATT_HOURS, \
    ICON_POWER, DEVICE_CLASS_VOLTAGE, DEVICE_CLASS_POWER, DEVICE_CLASS_ENERGY

AUTO_LOAD = ['modbus', 'text_sensor']

gfsun2000_ns = cg.esphome_ns.namespace('gfsun2000')
GFSun2000 = gfsun2000_ns.class_('GFSun2000', cg.PollingComponent, modbus.ModbusDevice)

CONF_DEVICE_ID = "device_id"
CONF_AC_VOLTAGE = "ac_voltage"
CONF_DC_VOLTAGE = "dc_voltage"
CONF_OUTPUT_POWER = "output_power"
CONF_CUSTOM_ENERGY = "custom_energy"
CONF_TOTAL_ENERGY = "total_energy"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(GFSun2000),
    cv.Optional(CONF_DEVICE_ID): text_sensor.text_sensor_schema(),
    cv.Optional(CONF_AC_VOLTAGE): sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        icon=ICON_FLASH,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_VOLTAGE
    ),
    cv.Optional(CONF_DC_VOLTAGE): sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        icon=ICON_FLASH,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_VOLTAGE
    ),
    cv.Optional(CONF_OUTPUT_POWER): sensor.sensor_schema(
        unit_of_measurement=UNIT_WATT,
        icon=ICON_POWER,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_POWER
    ),
    cv.Optional(CONF_CUSTOM_ENERGY): sensor.sensor_schema(
        unit_of_measurement=UNIT_WATT_HOURS,
        icon=ICON_POWER,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_ENERGY
    ),
    cv.Optional(CONF_TOTAL_ENERGY): sensor.sensor_schema(
        unit_of_measurement=UNIT_WATT_HOURS,
        icon=ICON_POWER,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_ENERGY
    ),
}).extend(cv.polling_component_schema('60s')).extend(modbus.modbus_device_schema(0x01))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await modbus.register_modbus_device(var, config)

    if CONF_DEVICE_ID in config:
        conf = config[CONF_DEVICE_ID]
        sens = cg.new_Pvariable(conf[CONF_ID])
        await text_sensor.register_text_sensor(sens, conf)
        cg.add(var.set_device_id_sensor(sens))
    if CONF_AC_VOLTAGE in config:
        conf = config[CONF_AC_VOLTAGE]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_ac_voltage_sensor(sens))
    if CONF_DC_VOLTAGE in config:
        conf = config[CONF_DC_VOLTAGE]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_dc_voltage_sensor(sens))
    if CONF_OUTPUT_POWER in config:
        conf = config[CONF_OUTPUT_POWER]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_output_power_sensor(sens))
    if CONF_CUSTOM_ENERGY in config:
        conf = config[CONF_CUSTOM_ENERGY]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_custom_energy_sensor(sens))
    if CONF_TOTAL_ENERGY in config:
        conf = config[CONF_TOTAL_ENERGY]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_total_energy_sensor(sens))
