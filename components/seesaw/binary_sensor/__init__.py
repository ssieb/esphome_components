import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import CONF_ID, CONF_PIN
from .. import seesaw_ns, Seesaw, CONF_SEESAW

SeesawBinarySensor = seesaw_ns.class_(
    "SeesawBinarySensor", binary_sensor.BinarySensor, cg.Component
)

CONFIG_SCHEMA = binary_sensor.binary_sensor_schema(SeesawBinarySensor).extend(
    {
        cv.GenerateID(CONF_SEESAW): cv.use_id(Seesaw),
        cv.Required(CONF_PIN): cv.int_range(0, 31),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await binary_sensor.register_binary_sensor(var, config)
    seesaw = await cg.get_variable(config[CONF_SEESAW])
    cg.add(var.set_parent(seesaw))
    cg.add(var.set_pin(config[CONF_PIN]))

