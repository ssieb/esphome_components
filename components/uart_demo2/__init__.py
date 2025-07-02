import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import uart, binary_sensor, button, output, sensor, switch, text_sensor
from esphome.const import CONF_ID, CONF_STATE, DEVICE_CLASS_VOLTAGE, ICON_FLASH, UNIT_VOLT

MULTI_CONF = True
DEPENDENCIES = ['uart']
AUTO_LOAD = ['binary_sensor', 'button', 'output', 'sensor', 'switch', 'text_sensor']

uart_demo_ns = cg.esphome_ns.namespace('uart_demo')

UARTDemo = uart_demo_ns.class_('UARTDemo', cg.Component, uart.UARTDevice)
UARTDemoBOutput = uart_demo_ns.class_("UARTDemoBOutput", output.BinaryOutput)
UARTDemoFOutput = uart_demo_ns.class_("UARTDemoFOutput", output.FloatOutput)
UARTDemoSwitch = uart_demo_ns.class_("UARTDemoSwitch", switch.Switch, cg.Component)
UARTDemoButton = uart_demo_ns.class_("UARTDemoButton", button.Button, cg.Component)

CONF_THE_TEXT = "the_text"
CONF_THE_SENSOR = "the_sensor"
CONF_THE_BIN_OUTPUT = "the_bin_output"
CONF_THE_FLT_OUTPUT = "the_flt_output"
CONF_THE_BINSENSOR = "the_binsensor"
CONF_THE_SWITCH = "the_switch"
CONF_THE_BUTTON = "the_button"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(UARTDemo),
    cv.Optional(CONF_THE_TEXT): text_sensor.text_sensor_schema(text_sensor.TextSensor),
    cv.Optional(CONF_THE_SENSOR): sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        icon=ICON_FLASH,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_VOLTAGE
    ),
    cv.Optional(CONF_THE_BIN_OUTPUT): output.BINARY_OUTPUT_SCHEMA.extend({cv.GenerateID(): cv.declare_id(UARTDemoBOutput)}),
    cv.Optional(CONF_THE_FLT_OUTPUT): output.FLOAT_OUTPUT_SCHEMA.extend({cv.GenerateID(): cv.declare_id(UARTDemoFOutput)}),
    cv.Optional(CONF_THE_BINSENSOR): binary_sensor.binary_sensor_schema(),
    cv.Optional(CONF_THE_SWITCH): switch.switch_schema(UARTDemoSwitch),
    cv.Optional(CONF_THE_BUTTON): button.button_schema(UARTDemoButton),
}).extend(uart.UART_DEVICE_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    if CONF_THE_TEXT in config:
        sens = await text_sensor.new_text_sensor(config[CONF_THE_TEXT])
        cg.add(var.set_the_text(sens))

    if CONF_THE_SENSOR in config:
        sens = await sensor.new_sensor(config[CONF_THE_SENSOR])
        cg.add(var.set_the_sensor(sens))

    if CONF_THE_BIN_OUTPUT in config:
        conf = config[CONF_THE_BIN_OUTPUT]
        out = cg.new_Pvariable(conf[CONF_ID])
        await output.register_output(out, conf)
        cg.add(out.set_parent(var))

    if CONF_THE_FLT_OUTPUT in config:
        conf = config[CONF_THE_FLT_OUTPUT]
        out = cg.new_Pvariable(conf[CONF_ID])
        await output.register_output(out, conf)
        cg.add(out.set_parent(var))

    if CONF_THE_BINSENSOR in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_THE_BINSENSOR])
        cg.add(var.set_the_binsensor(sens))

    if CONF_THE_SWITCH in config:
        sw = await switch.new_switch(config[CONF_THE_SWITCH])
        cg.add(sw.set_parent(var))

    if CONF_THE_BUTTON in config:
        btn = await button.new_button(config[CONF_THE_BUTTON])
        cg.add(btn.set_parent(var))

