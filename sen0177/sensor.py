import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import CONF_ID, CONF_PM_1_0, CONF_PM_2_5, CONF_PM_10_0, \
    CONF_PMC_0_5, CONF_PMC_1_0, CONF_PMC_2_5, CONF_PMC_10_0, \
    UNIT_MICROGRAMS_PER_CUBIC_METER, UNIT_COUNTS_PER_CUBIC_METER, \
    ICON_CHEMICAL_WEAPON, ICON_COUNTER

DEPENDENCIES = ['uart']

sen0177_ns = cg.esphome_ns.namespace('sen0177')
SEN0177Component = sen0177_ns.class_('SEN0177Component', uart.UARTDevice, cg.Component)

CONF_PMC_0_3 = 'pmc_0_3'
CONF_PMC_5_0 = 'pmc_5_0'

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(SEN0177Component),

    cv.Optional(CONF_PM_1_0): sensor.sensor_schema(UNIT_MICROGRAMS_PER_CUBIC_METER,
                                                   ICON_CHEMICAL_WEAPON, 2),
    cv.Optional(CONF_PM_2_5): sensor.sensor_schema(UNIT_MICROGRAMS_PER_CUBIC_METER,
                                                   ICON_CHEMICAL_WEAPON, 2),
    cv.Optional(CONF_PM_10_0): sensor.sensor_schema(UNIT_MICROGRAMS_PER_CUBIC_METER,
                                                    ICON_CHEMICAL_WEAPON, 2),
    cv.Optional(CONF_PMC_0_3): sensor.sensor_schema(UNIT_COUNTS_PER_CUBIC_METER,
                                                    ICON_COUNTER, 2),
    cv.Optional(CONF_PMC_0_5): sensor.sensor_schema(UNIT_COUNTS_PER_CUBIC_METER,
                                                    ICON_COUNTER, 2),
    cv.Optional(CONF_PMC_1_0): sensor.sensor_schema(UNIT_COUNTS_PER_CUBIC_METER,
                                                    ICON_COUNTER, 2),
    cv.Optional(CONF_PMC_2_5): sensor.sensor_schema(UNIT_COUNTS_PER_CUBIC_METER,
                                                    ICON_COUNTER, 2),
    cv.Optional(CONF_PMC_5_0): sensor.sensor_schema(UNIT_COUNTS_PER_CUBIC_METER,
                                                    ICON_COUNTER, 2),
    cv.Optional(CONF_PMC_10_0): sensor.sensor_schema(UNIT_COUNTS_PER_CUBIC_METER,
                                                     ICON_COUNTER, 2),
}).extend(cv.COMPONENT_SCHEMA).extend(uart.UART_DEVICE_SCHEMA)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield uart.register_uart_device(var, config)

    if CONF_PM_1_0 in config:
        sens = yield sensor.new_sensor(config[CONF_PM_1_0])
        cg.add(var.set_pm_1_0_sensor(sens))

    if CONF_PM_2_5 in config:
        sens = yield sensor.new_sensor(config[CONF_PM_2_5])
        cg.add(var.set_pm_2_5_sensor(sens))

    if CONF_PM_10_0 in config:
        sens = yield sensor.new_sensor(config[CONF_PM_10_0])
        cg.add(var.set_pm_10_0_sensor(sens))

    if CONF_PMC_0_3 in config:
        sens = yield sensor.new_sensor(config[CONF_PMC_0_3])
        cg.add(var.set_pmc_0_3_sensor(sens))

    if CONF_PMC_0_5 in config:
        sens = yield sensor.new_sensor(config[CONF_PMC_0_5])
        cg.add(var.set_pmc_0_5_sensor(sens))

    if CONF_PMC_1_0 in config:
        sens = yield sensor.new_sensor(config[CONF_PMC_1_0])
        cg.add(var.set_pmc_1_0_sensor(sens))

    if CONF_PMC_2_5 in config:
        sens = yield sensor.new_sensor(config[CONF_PMC_2_5])
        cg.add(var.set_pmc_2_5_sensor(sens))

    if CONF_PMC_5_0 in config:
        sens = yield sensor.new_sensor(config[CONF_PMC_5_0])
        cg.add(var.set_pmc_5_0_sensor(sens))

    if CONF_PMC_10_0 in config:
        sens = yield sensor.new_sensor(config[CONF_PMC_10_0])
        cg.add(var.set_pmc_10_0_sensor(sens))

