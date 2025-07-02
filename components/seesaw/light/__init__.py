import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light
from esphome.const import CONF_OUTPUT_ID, CONF_PIN
from .. import seesaw_ns, Seesaw, CONF_SEESAW

SeesawNeopixel = seesaw_ns.class_("SeesawNeopixel", light.LightOutput, cg.Component)

CONFIG_SCHEMA = light.light_schema(SeesawNeopixel, light.LightType.RGB).extend(
    {
        cv.GenerateID(CONF_SEESAW): cv.use_id(Seesaw),
        cv.Required(CONF_PIN): cv.int_range(0, 31),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await cg.register_component(var, config)
    await light.register_light(var, config)
    seesaw = await cg.get_variable(config[CONF_SEESAW])
    cg.add(var.set_parent(seesaw))
    cg.add(var.set_pin(config[CONF_PIN]))

