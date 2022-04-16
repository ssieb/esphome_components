import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import uart
from esphome.components import sensor
from esphome.const import CONF_ID, CONF_STATE, \
    UNIT_CELSIUS, UNIT_HERTZ, UNIT_MINUTE, UNIT_PERCENT, UNIT_VOLT, \
    ICON_COUNTER, ICON_CURRENT_AC, ICON_FLASH, ICON_PERCENT, ICON_THERMOMETER

DEPENDENCIES = ['uart']

apc_serial_ns = cg.esphome_ns.namespace('apc')

ApcSmartUpsSerial = apc_serial_ns.class_('ApcSmartUpsSerial', cg.PollingComponent, uart.UARTDevice)

CONF_INTERNAL_TEMP = "internal_temp"
CONF_BATTERY_VOLT = "battery_voltage"
CONF_BATTERY_LEVEL = "battery_level"
CONF_POWER_LOAD = "power_load"
CONF_INPUT_VOLT = "input_voltage"
CONF_OUTPUT_VOLT = "output_voltage"
CONF_LINE_FREQ = "line_frequency"
CONF_EST_RUNTIME = "estimated_runtime"

CONFIG_SCHEMA = uart.UART_DEVICE_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(ApcSmartUpsSerial),
    cv.Optional(CONF_INTERNAL_TEMP): sensor.sensor_schema(
        unit_of_measurement=UNIT_CELSIUS,
        icon=ICON_THERMOMETER,
        accuracy_decimals=1
    ),
    cv.Optional(CONF_BATTERY_VOLT): sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        icon=ICON_FLASH,
        accuracy_decimals=2
    ),
    cv.Optional(CONF_BATTERY_LEVEL): sensor.sensor_schema(
        unit_of_measurement=UNIT_PERCENT,
        icon=ICON_PERCENT,
        accuracy_decimals=1
    ),
    cv.Optional(CONF_POWER_LOAD): sensor.sensor_schema(
        unit_of_measurement=UNIT_PERCENT,
        icon=ICON_PERCENT,
        accuracy_decimals=1
    ),
    cv.Optional(CONF_INPUT_VOLT): sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        icon=ICON_FLASH,
        accuracy_decimals=1
    ),
    cv.Optional(CONF_OUTPUT_VOLT): sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        icon=ICON_FLASH,
        accuracy_decimals=1
    ),
    cv.Optional(CONF_LINE_FREQ): sensor.sensor_schema(
        unit_of_measurement=UNIT_HERTZ,
        icon=ICON_CURRENT_AC,
        accuracy_decimals=2
    ),
    cv.Optional(CONF_EST_RUNTIME): sensor.sensor_schema(
        unit_of_measurement=UNIT_MINUTE,
        icon=ICON_COUNTER,
        accuracy_decimals=0
    ),
}).extend(cv.polling_component_schema('60s'))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    if CONF_INTERNAL_TEMP in config:
        sens = await sensor.new_sensor(config[CONF_INTERNAL_TEMP])
        cg.add(var.set_internal_temp_sensor(sens))
    if CONF_BATTERY_VOLT in config:
        sens = await sensor.new_sensor(config[CONF_BATTERY_VOLT])
        cg.add(var.set_battery_volt_sensor(sens))
    if CONF_BATTERY_LEVEL in config:
        sens = await sensor.new_sensor(config[CONF_BATTERY_LEVEL])
        cg.add(var.set_battery_level_sensor(sens))
    if CONF_POWER_LOAD in config:
        sens = await sensor.new_sensor(config[CONF_POWER_LOAD])
        cg.add(var.set_power_load_sensor(sens))
    if CONF_INPUT_VOLT in config:
        sens = await sensor.new_sensor(config[CONF_INPUT_VOLT])
        cg.add(var.set_input_volt_sensor(sens))
    if CONF_OUTPUT_VOLT in config:
        sens = await sensor.new_sensor(config[CONF_OUTPUT_VOLT])
        cg.add(var.set_output_volt_sensor(sens))
    if CONF_LINE_FREQ in config:
        sens = await sensor.new_sensor(config[CONF_LINE_FREQ])
        cg.add(var.set_line_freq_sensor(sens))
    if CONF_EST_RUNTIME in config:
        sens = await sensor.new_sensor(config[CONF_EST_RUNTIME])
        cg.add(var.set_est_runtime_sensor(sens))

