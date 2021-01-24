import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display, i2c
from esphome.const import CONF_ID, CONF_LAMBDA

DEPENDENCIES = ['i2c']

ht16k33_alpha_ns = cg.esphome_ns.namespace('ht16k33_alpha')
HT16K33AlphaDisplay = ht16k33_alpha_ns.class_('HT16K33AlphaDisplay', cg.PollingComponent, i2c.I2CDevice)

CONF_SCROLL = "scroll"
CONF_SCROLL_SPEED = "scroll_speed"
CONF_SCROLL_DWELL = "scroll_dwell"
CONF_SCROLL_DELAY = "scroll_delay"
CONF_SECONDARY_DISPLAYS = "secondary_displays"

CONFIG_SECONDARY = cv.Schema({
    cv.GenerateID(): cv.declare_id(i2c.I2CDevice)
}).extend(i2c.i2c_device_schema(None))

CONFIG_SCHEMA = display.BASIC_DISPLAY_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(HT16K33AlphaDisplay),
    cv.Optional(CONF_SCROLL, default=False): cv.boolean,
    cv.Optional(CONF_SCROLL_SPEED, default='250ms'): cv.positive_time_period_milliseconds,
    cv.Optional(CONF_SCROLL_DWELL, default='2s'): cv.positive_time_period_milliseconds,
    cv.Optional(CONF_SCROLL_DELAY, default='3'): cv.float_range(min=1),
    cv.Optional(CONF_SECONDARY_DISPLAYS): cv.ensure_list(CONFIG_SECONDARY),
}).extend(cv.polling_component_schema('1s')).extend(i2c.i2c_device_schema(0x70))

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield display.register_display(var, config)
    yield i2c.register_i2c_device(var, config)

    if CONF_LAMBDA in config:
        lambda_ = yield cg.process_lambda(config[CONF_LAMBDA],
                                          [(HT16K33AlphaDisplay.operator('ref'), 'it')],
                                          return_type=cg.void)
        cg.add(var.set_writer(lambda_))
    if config[CONF_SCROLL]:
        cg.add(var.set_scroll(True))
        cg.add(var.set_scroll_speed(config[CONF_SCROLL_SPEED]))
        cg.add(var.set_scroll_dwell(config[CONF_SCROLL_DWELL]))
        cg.add(var.set_scroll_delay(int(config[CONF_SCROLL_DELAY] * config[CONF_SCROLL_SPEED].total_milliseconds)))
    if CONF_SECONDARY_DISPLAYS in config:
        for conf in config[CONF_SECONDARY_DISPLAYS]:
            disp = cg.new_Pvariable(conf[CONF_ID])
            yield i2c.register_i2c_device(disp, conf)
            cg.add(var.add_secondary_display(disp))

