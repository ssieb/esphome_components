import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import CONF_ID
from .. import M5_8encoder, m5_8encoder_ns, CONF_M5_8ENCODER

M5_8encoderToggle = m5_8encoder_ns.class_(
    "M5_8encoderToggle", binary_sensor.BinarySensor, cg.Component
)

CONFIG_SCHEMA = binary_sensor.binary_sensor_schema(M5_8encoderToggle).extend(
    {
        cv.GenerateID(CONF_M5_8ENCODER): cv.use_id(M5_8encoder),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await binary_sensor.register_binary_sensor(var, config)
    parent = await cg.get_variable(config[CONF_M5_8ENCODER])
    cg.add(parent.register_toggle_listener(var))

