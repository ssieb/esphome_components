import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light
from esphome.const import CONF_NUM_LEDS, CONF_OUTPUT_ID, CONF_PIN
from .. import seesaw_ns, Seesaw, CONF_SEESAW

SeesawNeopixel = seesaw_ns.class_("SeesawNeopixel", light.AddressableLight)

CONFIG_SCHEMA = light.ADDRESSABLE_LIGHT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(SeesawNeopixel),
        cv.GenerateID(CONF_SEESAW): cv.use_id(Seesaw),
        cv.Required(CONF_PIN): cv.int_range(0, 31),
        # Defaults to 1 to keep existing single-pixel boards (e.g. the plain QT Rotary
        # Encoder with NeoPixel) working unchanged; set higher for boards with more than
        # one NeoPixel, e.g. 4 on the Quad Rotary Encoder Breakout.
        cv.Optional(CONF_NUM_LEDS, default=1): cv.int_range(min=1, max=64),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await light.register_light(var, config)
    await cg.register_component(var, config)
    seesaw = await cg.get_variable(config[CONF_SEESAW])
    cg.add(var.set_parent(seesaw))
    cg.add(var.set_pin(config[CONF_PIN]))
    cg.add(var.set_num_leds(config[CONF_NUM_LEDS]))
