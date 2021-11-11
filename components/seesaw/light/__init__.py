import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light
from esphome.const import CONF_OUTPUT_ID
from .. import seesaw_ns, Seesaw, CONF_SEESAW

SeesawNeopixel = seesaw_ns.class_("SeesawNeopixel", light.LightOutput, cg.Component)

CONFIG_SCHEMA = light.RGB_LIGHT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(SeesawNeopixel),
        cv.GenerateID(CONF_SEESAW): cv.use_id(Seesaw),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await cg.register_component(var, config)
    await light.register_light(var, config)
    seesaw = await cg.get_variable(config[CONF_SEESAW])
    cg.add(var.set_parent(seesaw))

