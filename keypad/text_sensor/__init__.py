import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import text_sensor
from esphome.const import CONF_ID, CONF_MAX_LENGTH
from .. import Keypad, keypad_ns, CONF_KEYPAD_ID

CONF_END_KEYS = 'end_keys'
CONF_BACK_KEYS = 'back_keys'
CONF_ALLOWED_KEYS = 'allowed_keys'
CONF_ON_PROGRESS= 'on_progress'

DEPENDENCIES = ['keypad']

KeypadTextSensor = keypad_ns.class_('KeypadTextSensor', text_sensor.TextSensor, cg.Component)
KeypadProgressTrigger = keypad_ns.class_('KeypadProgressTrigger',
                                         automation.Trigger.template(cg.std_string))

CONFIG_SCHEMA = cv.All(text_sensor.TEXT_SENSOR_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(KeypadTextSensor),
    cv.GenerateID(CONF_KEYPAD_ID): cv.use_id(Keypad),
    cv.Optional(CONF_MAX_LENGTH): cv.int_,
    cv.Optional(CONF_END_KEYS): cv.string,
    cv.Optional(CONF_BACK_KEYS): cv.string,
    cv.Optional(CONF_ALLOWED_KEYS): cv.string,
    cv.Optional(CONF_ON_PROGRESS): automation.validate_automation(single=True),
}), cv.has_at_least_one_key(CONF_END_KEYS, CONF_MAX_LENGTH))


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield text_sensor.register_text_sensor(var, config)
    keypad = yield cg.get_variable(config[CONF_KEYPAD_ID])
    cg.add(keypad.register_listener(var))
    if CONF_MAX_LENGTH in config:
        cg.add(var.set_max_length(config[CONF_MAX_LENGTH]))
    if CONF_END_KEYS in config:
        cg.add(var.set_end_keys(config[CONF_END_KEYS]))
    if CONF_BACK_KEYS in config:
        cg.add(var.set_back_keys(config[CONF_BACK_KEYS]))
    if CONF_ALLOWED_KEYS in config:
        cg.add(var.set_allowed_keys(config[CONF_ALLOWED_KEYS]))
    if CONF_ON_PROGRESS in config:
        yield automation.build_automation(var.get_progress_trigger(), [(cg.std_string, 'x')],
                                          config[CONF_ON_PROGRESS])


