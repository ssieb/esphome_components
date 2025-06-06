import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_DELAY

MULTI_CONF = True

set_ns = cg.esphome_ns.namespace('boot_delay')
BootDelay = set_ns.class_('BootDelay', cg.Component)

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(BootDelay),
    cv.Required(CONF_DELAY): cv.positive_time_period_milliseconds,
})

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    cg.add(var.set_delay(config[CONF_DELAY]))
