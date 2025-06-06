import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display
from esphome.const import CONF_ID, CONF_LAMBDA
from .. import sn74hc595_ns, SN74HC595Component, CONF_SN74HC595

DEPENDENCIES = ['sn74hc595']

SN74HC595Display = sn74hc595_ns.class_('SN74HC595Display', cg.PollingComponent)

CONFIG_SCHEMA = display.BASIC_DISPLAY_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(SN74HC595Display),
    cv.GenerateID(CONF_SN74HC595): cv.use_id(SN74HC595Component)
}).extend(cv.polling_component_schema('1s'))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await display.register_display(var, config)
    parent = await cg.get_variable(config[CONF_SN74HC595])
    cg.add(var.set_parent(parent))

    if CONF_LAMBDA in config:
        lambda_ = await cg.process_lambda(config[CONF_LAMBDA],
                                          [(SN74HC595Display.operator('ref'), 'it')],
                                          return_type=cg.void)
        cg.add(var.set_writer(lambda_))

