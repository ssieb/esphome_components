import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import (
    CONF_ID,
    CONF_BATTERY_VOLTAGE,
    CONF_CAPACITY,
    CONF_CURRENT,
    CONF_NUMBER,
    CONF_TEMPERATURE,
    CONF_VOLTAGE,
    ICON_CURRENT_AC,
    ICON_FLASH, 
    ICON_GAUGE,
    ICON_PERCENT,
    ICON_THERMOMETER,
    UNIT_AMPERE,
    UNIT_CELSIUS,
    UNIT_PERCENT,
    UNIT_VOLT,
)

DEPENDENCIES = ['uart']

overkill_ns = cg.esphome_ns.namespace('overkill')
Overkill = overkill_ns.class_('Overkill', uart.UARTDevice, cg.PollingComponent)

CONF_STRING = "string"
CONF_BALANCE_CAPACITY = "balance_capacity"
CONF_RATE_CAPACITY = "rate_capacity"
UNIT_AMPHOUR = "Ah"

CONFIG_SCHEMA = uart.UART_DEVICE_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(Overkill),
    cv.Optional(CONF_BATTERY_VOLTAGE): cv.ensure_list(
        sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            accuracy_decimals=3
        ).extend({
            cv.Required(CONF_STRING): cv.positive_int
        })
    ),
    cv.Optional(CONF_TEMPERATURE): cv.ensure_list(
        sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            accuracy_decimals=1
        ).extend({
            cv.Required(CONF_NUMBER): cv.positive_int
        })
    ),
    cv.Optional(CONF_VOLTAGE): sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        icon=ICON_FLASH,
        accuracy_decimals=2
    ),
    cv.Optional(CONF_CURRENT): sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE,
        icon=ICON_CURRENT_AC,
        accuracy_decimals=2
    ),
    cv.Optional(CONF_BALANCE_CAPACITY): sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPHOUR,
        icon=ICON_GAUGE,
        accuracy_decimals=2
    ),
    cv.Optional(CONF_RATE_CAPACITY): sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPHOUR,
        icon=ICON_GAUGE,
        accuracy_decimals=2
    ),
    cv.Optional(CONF_CAPACITY): sensor.sensor_schema(
        unit_of_measurement=UNIT_PERCENT,
        icon=ICON_PERCENT,
        accuracy_decimals=0
    ),
}).extend(cv.polling_component_schema('60s'))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    if CONF_BATTERY_VOLTAGE in config:
        for conf in config[CONF_BATTERY_VOLTAGE]:
          sens = await sensor.new_sensor(conf)
          snum = conf[CONF_STRING]
          cg.add(var.add_battery_voltage_sensor(snum, sens))

    if CONF_TEMPERATURE in config:
        for conf in config[CONF_TEMPERATURE]:
          sens = await sensor.new_sensor(conf)
          snum = conf[CONF_NUMBER]
          cg.add(var.add_temperature_sensor(snum, sens))

    if CONF_VOLTAGE in config:
        sens = await sensor.new_sensor(config[CONF_VOLTAGE])
        cg.add(var.set_voltage_sensor(sens))

    if CONF_CURRENT in config:
        sens = await sensor.new_sensor(config[CONF_CURRENT])
        cg.add(var.set_current_sensor(sens))

    if CONF_BALANCE_CAPACITY in config:
        sens = await sensor.new_sensor(config[CONF_BALANCE_CAPACITY])
        cg.add(var.set_balance_capacity_sensor(sens))

    if CONF_RATE_CAPACITY in config:
        sens = await sensor.new_sensor(config[CONF_RATE_CAPACITY])
        cg.add(var.set_rate_capacity_sensor(sens))

    if CONF_CAPACITY in config:
        sens = await sensor.new_sensor(config[CONF_CAPACITY])
        cg.add(var.set_capacity_sensor(sens))
