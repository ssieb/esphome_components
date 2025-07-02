import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import CONF_ID, CONF_KEY
from .. import CardKB, cardkb_ns, CONF_CARDKB_ID

DEPENDENCIES = ['cardkb']

CardKBBinarySensor = cardkb_ns.class_('CardKBBinarySensor', binary_sensor.BinarySensor)

def key(value):
    if isinstance(value, str):
        if len(value) == 1:
            return ord(value[0])
    elif isinstance(value, int):
        if value > 0 and value <= 255:
            return value
        raise cv.Invalid("Number must be between 1 and 255")
    raise cv.Invalid("Must be a string with one character or a number")

CONFIG_SCHEMA = binary_sensor.binary_sensor_schema(CardKBBinarySensor).extend(
    {
        cv.GenerateID(CONF_CARDKB_ID): cv.use_id(CardKB),
        cv.Required(CONF_KEY): key,
    }
)


async def to_code(config):
    key = config[CONF_KEY]
    var = cg.new_Pvariable(config[CONF_ID], config[CONF_KEY])
    await binary_sensor.register_binary_sensor(var, config)
    cardkb = await cg.get_variable(config[CONF_CARDKB_ID])
    cg.add(cardkb.register_listener(var))
