import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, modbus
from esphome.const import CONF_ID, UNIT_VOLT, ICON_FLASH, UNIT_AMPERE, UNIT_WATT, \
    ICON_POWER, ICON_CURRENT_AC, UNIT_HERTZ, CONF_TEMPERATURE, ICON_THERMOMETER, UNIT_CELSIUS

AUTO_LOAD = ['modbus']

growatt_ns = cg.esphome_ns.namespace('growatt')
Growatt = growatt_ns.class_('Growatt', cg.PollingComponent, modbus.ModbusDevice)

CONF_INPUT_POWER = "input_power"
CONF_PV1_VOLTAGE = "pv1_voltage"
CONF_PV1_CURRENT = "pv1_current"
CONF_PV1_POWER = "pv1_power"
CONF_PV2_VOLTAGE = "pv2_voltage"
CONF_PV2_CURRENT = "pv2_current"
CONF_PV2_POWER = "pv2_power"
CONF_OUTPUT_POWER = "output_power"
CONF_GRID_FREQUENCY = "grid_frequency"
CONF_AC_VOLTAGE = "ac_voltage"
CONF_AC_CURRENT = "ac_current"
CONF_AC_POWER = "ac_power"
CONF_TODAY_GEN = "today_gen"
CONF_TOTAL_GEN = "total_gen"
CONF_TODAY_GRID = "today_grid"
CONF_TOTAL_GRID = "total_grid"
UNIT_KILOWATT_HOURS = "kWh"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(Growatt),
    cv.Optional(CONF_INPUT_POWER): sensor.sensor_schema(
        unit_of_measurement=UNIT_WATT,
        icon=ICON_POWER,
        accuracy_decimals=1
    ),
    cv.Optional(CONF_PV1_VOLTAGE): sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        icon=ICON_FLASH,
        accuracy_decimals=1
    ),
    cv.Optional(CONF_PV1_CURRENT): sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE,
        icon=ICON_CURRENT_AC,
        accuracy_decimals=3
    ),
    cv.Optional(CONF_PV1_POWER): sensor.sensor_schema(
        unit_of_measurement=UNIT_WATT,
        icon=ICON_POWER,
        accuracy_decimals=1
    ),
    cv.Optional(CONF_PV2_VOLTAGE): sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        icon=ICON_FLASH,
        accuracy_decimals=1
    ),
    cv.Optional(CONF_PV2_CURRENT): sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE,
        icon=ICON_CURRENT_AC,
        accuracy_decimals=3
    ),
    cv.Optional(CONF_PV2_POWER): sensor.sensor_schema(
        unit_of_measurement=UNIT_WATT,
        icon=ICON_POWER,
        accuracy_decimals=1
    ),
    cv.Optional(CONF_OUTPUT_POWER): sensor.sensor_schema(
        unit_of_measurement=UNIT_WATT,
        icon=ICON_POWER,
        accuracy_decimals=1
    ),
    cv.Optional(CONF_GRID_FREQUENCY): sensor.sensor_schema(
        unit_of_measurement=UNIT_HERTZ,
        icon=ICON_CURRENT_AC,
        accuracy_decimals=1
    ),
    cv.Optional(CONF_AC_VOLTAGE): sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        icon=ICON_FLASH,
        accuracy_decimals=1
    ),
    cv.Optional(CONF_AC_CURRENT): sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE,
        icon=ICON_CURRENT_AC,
        accuracy_decimals=3
    ),
    cv.Optional(CONF_AC_POWER): sensor.sensor_schema(
        unit_of_measurement=UNIT_WATT,
        icon=ICON_POWER,
        accuracy_decimals=1
    ),
    cv.Optional(CONF_TODAY_GEN): sensor.sensor_schema(
        unit_of_measurement=UNIT_KILOWATT_HOURS,
        icon=ICON_POWER,
        accuracy_decimals=1
    ),
    cv.Optional(CONF_TOTAL_GEN): sensor.sensor_schema(
        unit_of_measurement=UNIT_KILOWATT_HOURS,
        icon=ICON_POWER,
        accuracy_decimals=1
    ),
    cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
        unit_of_measurement=UNIT_CELSIUS,
        icon=ICON_THERMOMETER,
        accuracy_decimals=1
    ),
    cv.Optional(CONF_TODAY_GRID): sensor.sensor_schema(
        unit_of_measurement=UNIT_KILOWATT_HOURS,
        icon=ICON_POWER,
        accuracy_decimals=1
    ),
    cv.Optional(CONF_TOTAL_GRID): sensor.sensor_schema(
        unit_of_measurement=UNIT_KILOWATT_HOURS,
        icon=ICON_POWER,
        accuracy_decimals=1
    ),
}).extend(cv.polling_component_schema('60s')).extend(modbus.modbus_device_schema(0x01))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await modbus.register_modbus_device(var, config)

    if CONF_INPUT_POWER in config:
        conf = config[CONF_INPUT_POWER]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_input_power_sensor(sens))
    if CONF_PV1_VOLTAGE in config:
        conf = config[CONF_PV1_VOLTAGE]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_pv1_voltage_sensor(sens))
    if CONF_PV1_CURRENT in config:
        conf = config[CONF_PV1_CURRENT]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_pv1_current_sensor(sens))
    if CONF_PV1_POWER in config:
        conf = config[CONF_PV1_POWER]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_pv1_power_sensor(sens))
    if CONF_PV2_VOLTAGE in config:
        conf = config[CONF_PV2_VOLTAGE]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_pv2_voltage_sensor(sens))
    if CONF_PV2_CURRENT in config:
        conf = config[CONF_PV2_CURRENT]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_pv2_current_sensor(sens))
    if CONF_PV2_POWER in config:
        conf = config[CONF_PV2_POWER]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_pv2_power_sensor(sens))
    if CONF_OUTPUT_POWER in config:
        conf = config[CONF_OUTPUT_POWER]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_output_power_sensor(sens))
    if CONF_GRID_FREQUENCY in config:
        conf = config[CONF_GRID_FREQUENCY]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_grid_frequency_sensor(sens))
    if CONF_AC_VOLTAGE in config:
        conf = config[CONF_AC_VOLTAGE]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_ac_voltage_sensor(sens))
    if CONF_AC_CURRENT in config:
        conf = config[CONF_AC_CURRENT]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_ac_current_sensor(sens))
    if CONF_AC_POWER in config:
        conf = config[CONF_AC_POWER]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_ac_power_sensor(sens))
    if CONF_TODAY_GEN in config:
        conf = config[CONF_TODAY_GEN]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_today_gen_sensor(sens))
    if CONF_TOTAL_GEN in config:
        conf = config[CONF_TOTAL_GEN]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_total_gen_sensor(sens))
    if CONF_TEMPERATURE in config:
        conf = config[CONF_TEMPERATURE]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_temperature_sensor(sens))
    if CONF_TODAY_GRID in config:
        conf = config[CONF_TODAY_GRID]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_today_grid_sensor(sens))
    if CONF_TOTAL_GRID in config:
        conf = config[CONF_TOTAL_GRID]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_total_grid_sensor(sens))
