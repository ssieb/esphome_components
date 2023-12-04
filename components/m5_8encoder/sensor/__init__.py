import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from .. import M5_8encoder, m5_8encoder_ns, CONF_M5_8ENCODER
from esphome.const import (
    CONF_ID,
    CONF_MAX_VALUE,
    CONF_MIN_VALUE,
    CONF_NUMBER,
    STATE_CLASS_NONE,
    UNIT_STEPS,
    ICON_ROTATE_RIGHT,
)

M5_8encoderSensor = m5_8encoder_ns.class_("M5_8encoderSensor", sensor.Sensor, cg.Component)

CONFIG_SCHEMA = sensor.sensor_schema(
    unit_of_measurement=UNIT_STEPS,
    icon=ICON_ROTATE_RIGHT,
    accuracy_decimals=0,
    state_class=STATE_CLASS_NONE,
).extend(
    {
        cv.GenerateID(): cv.declare_id(M5_8encoderSensor),
        cv.GenerateID(CONF_M5_8ENCODER): cv.use_id(M5_8encoder),
        cv.Required(CONF_NUMBER): cv.int_range(0, 7),
        cv.Optional(CONF_MIN_VALUE): cv.int_,
        cv.Optional(CONF_MAX_VALUE): cv.int_,
    }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
    parent = await cg.get_variable(config[CONF_M5_8ENCODER])
    cg.add(parent.register_encoder_listener(var))
    cg.add(var.set_number(config[CONF_NUMBER]))
    if CONF_MIN_VALUE in config:
        cg.add(var.set_min_value(config[CONF_MIN_VALUE]))
    if CONF_MAX_VALUE in config:
        cg.add(var.set_max_value(config[CONF_MAX_VALUE]))

