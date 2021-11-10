import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from .. import Seesaw, seesaw_ns, CONF_SEESAW
from esphome.const import (
    CONF_ID,
    CONF_MIN_VALUE,
    CONF_MAX_VALUE,
    STATE_CLASS_NONE,
    UNIT_STEPS,
    ICON_ROTATE_RIGHT,
)

SeesawRotaryEncoder = seesaw_ns.class_("SeesawRotaryEncoder", sensor.Sensor, cg.Component)

CONFIG_SCHEMA = sensor.sensor_schema(
        unit_of_measurement=UNIT_STEPS,
        icon=ICON_ROTATE_RIGHT,
        accuracy_decimals=0,
        state_class=STATE_CLASS_NONE,
).extend(
    {
        cv.GenerateID(): cv.declare_id(SeesawRotaryEncoder),
        cv.GenerateID(CONF_SEESAW): cv.use_id(Seesaw),
        cv.Optional(CONF_MIN_VALUE): cv.int_,
        cv.Optional(CONF_MAX_VALUE): cv.int_,

    }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
    seesaw = await cg.get_variable(config[CONF_SEESAW])
    cg.add(var.set_parent(seesaw))
    if CONF_MIN_VALUE in config:
        cg.add(var.set_min_value(config[CONF_MIN_VALUE]))
    if CONF_MAX_VALUE in config:
        cg.add(var.set_max_value(config[CONF_MAX_VALUE]))


