import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart, binary_sensor
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

MULTI_CONF = True
DEPENDENCIES = ['uart']
AUTO_LOAD = ['sensor', 'binary_sensor']

jbd_bms_ns = cg.esphome_ns.namespace('jbd_bms')
JBD_BMS = jbd_bms_ns.class_('JBD_BMS', uart.UARTDevice, cg.PollingComponent)

CONF_STRING = "string"
CONF_BALANCE_CAPACITY = "balance_capacity"
CONF_RATE_CAPACITY = "rate_capacity"
CONF_CHARGE_FET = "charge_fet"
CONF_DISCHARGE_FET = "discharge_fet"
CONF_PROT_COVP = "covp_prot"
CONF_PROT_CUVP = "cuvp_prot"
CONF_PROT_POVP = "povp_prot"
CONF_PROT_PUVP = "puvp_prot"
CONF_PROT_CHGOT = "chgot_prot"
CONF_PROT_CHGUT = "chgut_prot"
CONF_PROT_DSGOT = "dsgot_prot"
CONF_PROT_DSGUT = "dsgut_prot"
CONF_PROT_CHGOC = "chgoc_prot"
CONF_PROT_DSGOC = "dsgoc_prot"
CONF_PROT_SHORT = "short_prot"
CONF_PROT_AFE = "afe_prot"
CONF_SWLOCK = "swlock"
CONF_BALANCE_STATE = "balance_state"
UNIT_AMPHOUR = "Ah"

CONFIG_SCHEMA = uart.UART_DEVICE_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(JBD_BMS),
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
    cv.Optional(CONF_CHARGE_FET): binary_sensor.binary_sensor_schema(),
    cv.Optional(CONF_DISCHARGE_FET): binary_sensor.binary_sensor_schema(),
    cv.Optional(CONF_PROT_COVP): binary_sensor.binary_sensor_schema(),
    cv.Optional(CONF_PROT_CUVP): binary_sensor.binary_sensor_schema(),
    cv.Optional(CONF_PROT_POVP): binary_sensor.binary_sensor_schema(),
    cv.Optional(CONF_PROT_PUVP): binary_sensor.binary_sensor_schema(),
    cv.Optional(CONF_PROT_CHGOT): binary_sensor.binary_sensor_schema(),
    cv.Optional(CONF_PROT_CHGUT): binary_sensor.binary_sensor_schema(),
    cv.Optional(CONF_PROT_DSGOT): binary_sensor.binary_sensor_schema(),
    cv.Optional(CONF_PROT_DSGUT): binary_sensor.binary_sensor_schema(),
    cv.Optional(CONF_PROT_CHGOC): binary_sensor.binary_sensor_schema(),
    cv.Optional(CONF_PROT_DSGOC): binary_sensor.binary_sensor_schema(),
    cv.Optional(CONF_PROT_SHORT): binary_sensor.binary_sensor_schema(),
    cv.Optional(CONF_PROT_AFE): binary_sensor.binary_sensor_schema(),
    cv.Optional(CONF_SWLOCK): binary_sensor.binary_sensor_schema(),
    cv.Optional(CONF_BALANCE_STATE): cv.ensure_list(
        binary_sensor.binary_sensor_schema().extend({
            cv.Required(CONF_STRING): cv.positive_int
        })
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

    if CONF_CHARGE_FET in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_CHARGE_FET])
        cg.add(var.set_charge_sensor(sens))

    if CONF_DISCHARGE_FET in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_DISCHARGE_FET])
        cg.add(var.set_discharge_sensor(sens))

    if CONF_PROT_COVP in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_PROT_COVP])
        cg.add(var.set_prot_covp_sensor(sens))

    if CONF_PROT_CUVP in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_PROT_CUVP])
        cg.add(var.set_prot_cuvp_sensor(sens))

    if CONF_PROT_POVP in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_PROT_POVP])
        cg.add(var.set_prot_povp_sensor(sens))

    if CONF_PROT_PUVP in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_PROT_PUVP])
        cg.add(var.set_prot_puvp_sensor(sens))

    if CONF_PROT_CHGOT in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_PROT_CHGOT])
        cg.add(var.set_prot_chgot_sensor(sens))

    if CONF_PROT_CHGUT in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_PROT_CHGUT])
        cg.add(var.set_prot_chgut_sensor(sens))

    if CONF_PROT_DSGOT in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_PROT_DSGOT])
        cg.add(var.set_prot_dsgot_sensor(sens))

    if CONF_PROT_DSGUT in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_PROT_DSGUT])
        cg.add(var.set_prot_dsgut_sensor(sens))

    if CONF_PROT_CHGOC in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_PROT_CHGOC])
        cg.add(var.set_prot_chgoc_sensor(sens))

    if CONF_PROT_DSGOC in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_PROT_DSGOC])
        cg.add(var.set_prot_dsgoc_sensor(sens))

    if CONF_PROT_SHORT in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_PROT_SHORT])
        cg.add(var.set_prot_short_sensor(sens))

    if CONF_PROT_AFE in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_PROT_AFE])
        cg.add(var.set_prot_afe_sensor(sens))

    if CONF_SWLOCK in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_SWLOCK])
        cg.add(var.set_swlock_sensor(sens))

    if CONF_BALANCE_STATE in config:
        for conf in config[CONF_BALANCE_STATE]:
          sens = await binary_sensor.new_binary_sensor(conf)
          snum = conf[CONF_STRING]
          cg.add(var.add_balance_state_sensor(snum, sens))
