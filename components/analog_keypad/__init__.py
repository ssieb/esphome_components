import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import key_provider, sensor, voltage_sampler
from esphome.const import (
    CONF_ID,
    CONF_KEY,
    CONF_SENSOR,
    CONF_VOLTAGE,
)

CODEOWNERS = ["@ssieb"]

AUTO_LOAD = ["key_provider", "voltage_sampler"]

MULTI_CONF = True

analog_keypad_ns = cg.esphome_ns.namespace("analog_keypad")
AnalogKeypad = analog_keypad_ns.class_(
    "AnalogKeypad", key_provider.KeyProvider, cg.Component
)

CONF_KEYPAD_ID = "keypad_id"
CONF_KEYS = "keys"
CONF_DEBOUNCE_TIME = "debounce_time"
CONF_VARIANCE = "variance"


def check_key(value):
    if len(value) != 1:
        raise cv.Invalid("Key must be one character")
    return value

CONFIG_SCHEMA = cv.All(
    cv.COMPONENT_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(AnalogKeypad),
            cv.Required(CONF_SENSOR): cv.use_id(voltage_sampler.VoltageSampler),
            cv.Optional(CONF_KEYS): cv.ensure_list(
                {
                    cv.Required(CONF_VOLTAGE): cv.positive_float,
                    cv.Optional(CONF_VARIANCE, default=0.05): cv.positive_float,
                    cv.Optional(CONF_KEY): cv.All(cv.string, check_key),
                }
            ),
            cv.Optional(CONF_DEBOUNCE_TIME, default=1): cv.int_range(min=1, max=100),
        }
    ),
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    sens = await cg.get_variable(config[CONF_SENSOR])
    cg.add(var.set_source(sens))
    for conf in config[CONF_KEYS]:
        voltage = conf[CONF_VOLTAGE]
        variance = conf[CONF_VARIANCE]
        if key := conf.get(CONF_KEY):
            cg.add(var.add_key(voltage, variance, key))
        else:
            cg.add(var.add_key(voltage, variance))
    cg.add(var.set_debounce_time(config[CONF_DEBOUNCE_TIME]))
