import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import CONF_ID
from .. import CONF_PHILIPS_ID, philips_series_3200_ns, PhilipsSeries3200

DEPENDENCIES = ['philips_espresso']

CONF_CLEAN_DURING_START = 'clean_during_start'

PowerSwitch = philips_series_3200_ns.class_(
    'Power', switch.Switch, cg.Component)

CONFIG_SCHEMA = switch.switch_schema(TemplateSwitch).extend(
    {
        cv.GenerateID(): cv.declare_id(PowerSwitch),
        cv.Required(CONF_PHILIPS_ID,): cv.use_id(PhilipsSeries3200),
        cv.Optional(CONF_CLEAN_DURING_START, default=True): cv.boolean
    }
).extend(cv.COMPONENT_SCHEMA)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield switch.register_switch(var, config)

    controller = yield cg.get_variable(config[CONF_PHILIPS_ID,])
    cg.add(var.set_cleaning(config[CLEAN_DURING_START]))
    cg.add(controller.register_power_switch(var))
