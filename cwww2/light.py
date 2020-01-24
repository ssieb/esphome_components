import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light, output
from esphome.const import CONF_OUTPUT_ID, CONF_COLOR_TEMPERATURE, CONF_BRIGHTNESS, \
    CONF_COLD_WHITE_COLOR_TEMPERATURE, CONF_WARM_WHITE_COLOR_TEMPERATURE

cwww2_ns = cg.esphome_ns.namespace('cwww2')
CWWW2LightOutput = cwww2_ns.class_('CWWW2LightOutput', light.LightOutput)

CONFIG_SCHEMA = light.RGB_LIGHT_SCHEMA.extend({
    cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(CWWW2LightOutput),
    cv.Required(CONF_COLOR_TEMPERATURE): cv.use_id(output.FloatOutput),
    cv.Required(CONF_BRIGHTNESS): cv.use_id(output.FloatOutput),
    cv.Required(CONF_COLD_WHITE_COLOR_TEMPERATURE): cv.color_temperature,
    cv.Required(CONF_WARM_WHITE_COLOR_TEMPERATURE): cv.color_temperature,
})


def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    yield light.register_light(var, config)
    ctemp = yield cg.get_variable(config[CONF_COLOR_TEMPERATURE])
    cg.add(var.set_color_temperature_output(ctemp))
    bright = yield cg.get_variable(config[CONF_BRIGHTNESS])
    cg.add(var.set_brightness_output(bright))
    cg.add(var.set_cold_white_temperature(config[CONF_COLD_WHITE_COLOR_TEMPERATURE]))
    cg.add(var.set_warm_white_temperature(config[CONF_WARM_WHITE_COLOR_TEMPERATURE]))
