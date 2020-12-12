import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, modbus
from esphome.const import CONF_ID, UNIT_VOLT, ICON_FLASH, UNIT_AMPERE, UNIT_WATT, \
    ICON_POWER, ICON_CURRENT_AC, UNIT_HERTZ, CONF_TEMPERATURE, ICON_THERMOMETER, UNIT_CELSIUS

AUTO_LOAD = ['modbus']

growatt_ns = cg.esphome_ns.namespace('growatt')
Growatt = growatt_ns.class_('Growatt', cg.PollingComponent, modbus.ModbusDevice)

CONF_INPUT_POWER_HIGH = "input_power_high"
CONF_INPUT_POWER_LOW = "input_power_low"
CONF_PV1_VOLTAGE = "pv1_voltage"
CONF_PV1_CURRENT = "pv1_current"
CONF_PV1_POWER_HIGH = "pv1_power_high"
CONF_PV1_POWER_LOW = "pv1_power_low"
CONF_PV2_VOLTAGE = "pv2_voltage"
CONF_PV2_CURRENT = "pv2_current"
CONF_PV2_POWER_HIGH = "pv2_power_high"
CONF_PV2_POWER_LOW = "pv2_power_low"
CONF_GRID_FREQUENCY = "grid_frequency"
CONF_OUTPUT_POWER_HIGH = "output_power_high"
CONF_OUTPUT_POWER_LOW = "output_power_low"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(Growatt),
    cv.Optional(CONF_INPUT_POWER_HIGH): sensor.sensor_schema(UNIT_WATT, ICON_POWER, 1),
    cv.Optional(CONF_INPUT_POWER_LOW): sensor.sensor_schema(UNIT_WATT, ICON_POWER, 1),
    cv.Optional(CONF_PV1_VOLTAGE): sensor.sensor_schema(UNIT_VOLT, ICON_FLASH, 1),
    cv.Optional(CONF_PV1_CURRENT): sensor.sensor_schema(UNIT_AMPERE, ICON_CURRENT_AC, 3),
    cv.Optional(CONF_PV1_POWER_HIGH): sensor.sensor_schema(UNIT_WATT, ICON_POWER, 1),
    cv.Optional(CONF_PV1_POWER_LOW): sensor.sensor_schema(UNIT_WATT, ICON_POWER, 1),
    cv.Optional(CONF_PV2_VOLTAGE): sensor.sensor_schema(UNIT_VOLT, ICON_FLASH, 1),
    cv.Optional(CONF_PV2_CURRENT): sensor.sensor_schema(UNIT_AMPERE, ICON_CURRENT_AC, 3),
    cv.Optional(CONF_PV2_POWER_HIGH): sensor.sensor_schema(UNIT_WATT, ICON_POWER, 1),
    cv.Optional(CONF_PV2_POWER_LOW): sensor.sensor_schema(UNIT_WATT, ICON_POWER, 1),
    cv.Optional(CONF_GRID_FREQUENCY): sensor.sensor_schema(UNIT_HERTZ, ICON_CURRENT_AC, 1),
    cv.Optional(CONF_OUTPUT_POWER_HIGH): sensor.sensor_schema(UNIT_WATT, ICON_POWER, 1),
    cv.Optional(CONF_OUTPUT_POWER_LOW): sensor.sensor_schema(UNIT_WATT, ICON_POWER, 1),
    cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(UNIT_CELSIUS, ICON_THERMOMETER, 1),
}).extend(cv.polling_component_schema('60s')).extend(modbus.modbus_device_schema(0x01))


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield modbus.register_modbus_device(var, config)

    if CONF_INPUT_POWER_HIGH in config:
        conf = config[CONF_INPUT_POWER_HIGH]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_input_power_high_sensor(sens))
    if CONF_INPUT_POWER_LOW in config:
        conf = config[CONF_INPUT_POWER_LOW]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_input_power_low_sensor(sens))
    if CONF_PV1_VOLTAGE in config:
        conf = config[CONF_PV1_VOLTAGE]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_pv1_voltage_sensor(sens))
    if CONF_PV1_CURRENT in config:
        conf = config[CONF_PV1_CURRENT]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_pv1_current_sensor(sens))
    if CONF_PV1_POWER_HIGH in config:
        conf = config[CONF_PV1_POWER_HIGH]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_pv1_power_high_sensor(sens))
    if CONF_PV1_POWER_LOW in config:
        conf = config[CONF_PV1_POWER_LOW]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_pv1_power_low_sensor(sens))
    if CONF_PV2_VOLTAGE in config:
        conf = config[CONF_PV2_VOLTAGE]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_pv2_voltage_sensor(sens))
    if CONF_PV2_CURRENT in config:
        conf = config[CONF_PV2_CURRENT]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_pv2_current_sensor(sens))
    if CONF_PV2_POWER_HIGH in config:
        conf = config[CONF_PV2_POWER_HIGH]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_pv2_power_high_sensor(sens))
    if CONF_PV2_POWER_LOW in config:
        conf = config[CONF_PV2_POWER_LOW]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_pv2_power_low_sensor(sens))
    if CONF_GRID_FREQUENCY in config:
        conf = config[CONF_GRID_FREQUENCY]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_grid_frequency_sensor(sens))
    if CONF_OUTPUT_POWER_HIGH in config:
        conf = config[CONF_OUTPUT_POWER_HIGH]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_output_power_high_sensor(sens))
    if CONF_OUTPUT_POWER_LOW in config:
        conf = config[CONF_OUTPUT_POWER_LOW]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_output_power_low_sensor(sens))
    if CONF_TEMPERATURE in config:
        conf = config[CONF_TEMPERATURE]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_temperature_sensor(sens))
