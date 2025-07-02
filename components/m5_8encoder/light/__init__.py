import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light
from esphome.const import CONF_NUMBER, CONF_OUTPUT_ID
from .. import M5_8encoder, m5_8encoder_ns, CONF_M5_8ENCODER

M5_8encoderLED = m5_8encoder_ns.class_("M5_8encoderLED", light.LightOutput, cg.Component)

CONFIG_SCHEMA = light.light_schema(M5_8encoderLED, light.LightType.RGB).extend(
    {
        cv.GenerateID(CONF_M5_8ENCODER): cv.use_id(M5_8encoder),
        cv.Required(CONF_NUMBER): cv.int_range(0, 8),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await cg.register_component(var, config)
    await light.register_light(var, config)
    parent = await cg.get_variable(config[CONF_M5_8ENCODER])
    cg.add(var.set_parent(parent))
    cg.add(var.set_number(config[CONF_NUMBER]))

