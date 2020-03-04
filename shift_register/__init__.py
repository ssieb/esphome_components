import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.const import CONF_ID, CONF_NUM_CHIPS, CONF_CLOCK_PIN, CONF_DATA_PIN, CONF_NUMBER, CONF_INVERTED

MULTI_CONF = True

shift_register_ns = cg.esphome_ns.namespace('shift_register')
ShiftRegister = shift_register_ns.class_('ShiftRegister', cg.Component)
ShiftRegisterGPIOPin = shift_register_ns.class_('ShiftRegisterGPIOPin', cg.GPIOPin)

CONF_LATCH_PIN = "latch_pin"
CONFIG_SCHEMA = cv.Schema({
    cv.Required(CONF_ID): cv.declare_id(ShiftRegister),
    cv.Optional(CONF_NUM_CHIPS): cv.int_,
    cv.Required(CONF_DATA_PIN): pins.gpio_output_pin_schema,
    cv.Required(CONF_CLOCK_PIN): pins.gpio_output_pin_schema,
    cv.Optional(CONF_LATCH_PIN): pins.gpio_output_pin_schema,
}).extend(cv.COMPONENT_SCHEMA)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    if CONF_NUM_CHIPS in config:
        cg.add(var.set_count(config[CONF_NUM_CHIPS]))
    pin = yield cg.gpio_pin_expression(config[CONF_DATA_PIN])
    cg.add(var.set_data_pin(pin))
    pin = yield cg.gpio_pin_expression(config[CONF_CLOCK_PIN])
    cg.add(var.set_clock_pin(pin))
    if CONF_LATCH_PIN in config:
        pin = yield cg.gpio_pin_expression(config[CONF_LATCH_PIN])
        cg.add(var.set_latch_pin(pin))



CONF_SHIFT_REGISTER = 'shift_register'
SHIFT_REGISTER_OUTPUT_PIN_SCHEMA = cv.Schema({
    cv.Required(CONF_SHIFT_REGISTER): cv.use_id(ShiftRegister),
    cv.Required(CONF_NUMBER): cv.int_,
    cv.Optional(CONF_INVERTED, default=False): cv.boolean,
})


@pins.PIN_SCHEMA_REGISTRY.register(CONF_SHIFT_REGISTER,
                                   (SHIFT_REGISTER_OUTPUT_PIN_SCHEMA, None))
def shift_register_pin_to_code(config):
    parent = yield cg.get_variable(config[CONF_SHIFT_REGISTER])
    yield ShiftRegisterGPIOPin.new(parent, config[CONF_NUMBER], config[CONF_INVERTED])

