import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import CONF_ID, CONF_INDEX, CONF_KEY
from .. import AnalogKeypad, analog_keypad_ns, CONF_KEYPAD_ID

DEPENDENCIES = ["analog_keypad"]

AnalogKeypadBinarySensor = analog_keypad_ns.class_(
    "AnalogKeypadBinarySensor", binary_sensor.BinarySensor
)


CONFIG_SCHEMA = cv.All(
    binary_sensor.binary_sensor_schema(AnalogKeypadBinarySensor).extend(
        {
            cv.GenerateID(CONF_KEYPAD_ID): cv.use_id(AnalogKeypad),
            cv.Optional(CONF_INDEX): cv.positive_int,
            cv.Optional(CONF_KEY): cv.string,
        }
    ),
    cv.has_exactly_one_key(CONF_INDEX, CONF_KEY),
)


async def to_code(config):
    if CONF_KEY in config:
        var = cg.new_Pvariable(config[CONF_ID])
        cg.add(var.set_key(ord(config[CONF_KEY][0])))
    else:
        var = cg.new_Pvariable(config[CONF_ID])
        cg.add(var.set_index(config[CONF_INDEX]))
    await binary_sensor.register_binary_sensor(var, config)
    analog_keypad = await cg.get_variable(config[CONF_KEYPAD_ID])
    cg.add(analog_keypad.register_listener(var))
