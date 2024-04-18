import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import fan
from esphome.const import CONF_OUTPUT_ID
from .. import puroair_ns, PuroAir, CONF_PUROAIR_ID

PuroAirFan = puroair_ns.class_(
    "PuroAirFan",
    cg.Component,
    fan.Fan,
)

CONFIG_SCHEMA = fan.FAN_SCHEMA.extend(
    {
        cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(PuroAirFan),
        cv.GenerateID(CONF_PUROAIR_ID): cv.use_id(PuroAir),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await cg.register_component(var, config)
    await fan.register_fan(var, config)
    parent = await cg.get_variable(config[CONF_PUROAIR_ID])
    cg.add(var.set_parent(parent))

