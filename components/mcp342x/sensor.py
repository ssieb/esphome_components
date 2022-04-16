import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, voltage_sampler
from esphome.const import CONF_GAIN, CONF_NBITS, CONF_CHANNEL, ICON_FLASH, UNIT_VOLT, CONF_ID
from . import mcp342x_ns, MCP342XComponent

DEPENDENCIES = ['mcp342x']

gains = [1, 2, 4, 8]

def validate_gain(value):
    if not isinstance(value, int) or not value in gains:
      raise cv.Invalid(f'invalid gain "{value}"')
    return gains.index(value)

bits = [12, 14, 16, 18]

def validate_resolution(value):
    if not isinstance(value, int) or not value in bits:
      raise cv.Invalid(f'invalid number of bits "{value}"')
    return bits.index(value)

MCP342XSensor = mcp342x_ns.class_('MCP342XSensor', sensor.Sensor, cg.PollingComponent,
                                  voltage_sampler.VoltageSampler)

CONF_MCP342X_ID = 'mcp342x_id'
CONFIG_SCHEMA = sensor.sensor_schema(
    MCP342XSensor,
    unit_of_measurement=UNIT_VOLT,
    icon=ICON_FLASH,
    accuracy_decimals=3
).extend({
    cv.GenerateID(CONF_MCP342X_ID): cv.use_id(MCP342XComponent),
    cv.Required(CONF_CHANNEL): cv.int_range(0, 3),
    cv.Required(CONF_NBITS): validate_resolution,
    cv.Optional(CONF_GAIN, 1): validate_gain,
}).extend(cv.polling_component_schema('60s'))


async def to_code(config):
    paren = await cg.get_variable(config[CONF_MCP342X_ID])
    var = cg.new_Pvariable(config[CONF_ID], paren)
    await sensor.register_sensor(var, config)
    await cg.register_component(var, config)

    cg.add(var.set_channel(config[CONF_CHANNEL]))
    cg.add(var.set_gain(config[CONF_GAIN]))
    cg.add(var.set_resolution(config[CONF_NBITS]))

    cg.add(paren.register_sensor(var))
