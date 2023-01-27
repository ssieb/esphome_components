import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor, sensor, uart
from esphome.const import CONF_ID, CONF_HEIGHT

DEPENDENCIES = ['uart']

AUTO_LOAD = ['sensor', 'binary_sensor']

jsdrive_ns = cg.esphome_ns.namespace('jsdrive')

JSDrive = jsdrive_ns.class_('JSDrive', cg.Component)

CONF_REMOTE_UART = "remote_uart"
CONF_DESK_UART = "desk_uart"
CONF_UP = "up"
CONF_DOWN = "down"
CONF_MEMORY1 = "memory1"
CONF_MEMORY2 = "memory2"
CONF_MEMORY3 = "memory3"

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(JSDrive),
    cv.Optional(CONF_REMOTE_UART): cv.use_id(uart.UARTComponent),
    cv.Optional(CONF_DESK_UART): cv.use_id(uart.UARTComponent),
    cv.Optional(CONF_HEIGHT): sensor.sensor_schema(
        accuracy_decimals = 1
    ),
    cv.Optional(CONF_UP): binary_sensor.binary_sensor_schema(),
    cv.Optional(CONF_DOWN): binary_sensor.binary_sensor_schema(),
    cv.Optional(CONF_MEMORY1): binary_sensor.binary_sensor_schema(),
    cv.Optional(CONF_MEMORY2): binary_sensor.binary_sensor_schema(),
    cv.Optional(CONF_MEMORY3): binary_sensor.binary_sensor_schema(),
})


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    if CONF_REMOTE_UART in config:
        remote_uart = await cg.get_variable(config[CONF_REMOTE_UART])
        cg.add(var.set_remote_uart(remote_uart))
    if CONF_DESK_UART in config:
        desk_uart = await cg.get_variable(config[CONF_DESK_UART])
        cg.add(var.set_desk_uart(desk_uart))
    if CONF_HEIGHT in config:
        sens = await sensor.new_sensor(config[CONF_HEIGHT])
        cg.add(var.set_height_sensor(sens))
    if CONF_UP in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_UP])
        cg.add(var.set_up_bsensor(sens))
    if CONF_DOWN in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_DOWN])
        cg.add(var.set_down_bsensor(sens))
    if CONF_MEMORY1 in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_MEMORY1])
        cg.add(var.set_memory1_bsensor(sens))
    if CONF_MEMORY2 in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_MEMORY2])
        cg.add(var.set_memory2_bsensor(sens))
    if CONF_MEMORY3 in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_MEMORY3])
        cg.add(var.set_memory3_bsensor(sens))

