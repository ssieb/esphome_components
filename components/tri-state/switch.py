import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import switch
from esphome.const import CONF_ID, CONF_ON_STATE, CONF_PIN

tri_state_ns = cg.esphome_ns.namespace("tri_state")
TriStateSwitch = tri_state_ns.class_("TriStateSwitch", switch.Switch, cg.Component)

CONFIG_SCHEMA = switch.switch_schema(TriStateSwitch).extend(
    {
        cv.Required(CONF_PIN): pins.gpio_output_pin_schema,
        cv.Required(CONF_ON_STATE): cv.boolean,
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await switch.register_switch(var, config)

    pin = await cg.gpio_pin_expression(config[CONF_PIN])
    cg.add(var.set_pin(pin))
    cg.add(var.set_on_state(config[CONF_ON_STATE]))

