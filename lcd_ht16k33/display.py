import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display, i2c
from esphome.const import CONF_ID, CONF_LAMBDA

DEPENDENCIES = ['i2c']

lcd_ht16k33_ns = cg.esphome_ns.namespace('lcd_ht16k33')
HT16K33LCDDisplay = lcd_ht16k33_ns.class_('HT16K33LCDDisplay', cg.PollingComponent, i2c.I2CDevice)

CONFIG_SCHEMA = display.BASIC_DISPLAY_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(HT16K33LCDDisplay)
}).extend(cv.polling_component_schema('1s')).extend(i2c.i2c_device_schema(0x70))

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield display.register_display(var, config)
    yield i2c.register_i2c_device(var, config)

    if CONF_LAMBDA in config:
        lambda_ = yield cg.process_lambda(config[CONF_LAMBDA],
                                          [(HT16K33LCDDisplay.operator('ref'), 'it')],
                                          return_type=cg.void)
        cg.add(var.set_writer(lambda_))

