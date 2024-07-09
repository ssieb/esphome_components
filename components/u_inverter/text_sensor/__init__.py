import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from esphome.const import (
    CONF_ID,
)
from .. import (
    u_inverter_ns,
    UInverter,
    CONF_U_INVERTER_ID,
)

UInverterTextSensor = u_inverter_ns.class_("UInverterTextSensor", cg.Component)

CONF_OPERATING_MODE = "operating_mode"

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(UInverterTextSensor),
        cv.GenerateID(CONF_U_INVERTER_ID): cv.use_id(UInverter),

        cv.Optional(CONF_OPERATING_MODE): text_sensor.text_sensor_schema(),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    parent = await cg.get_variable(config[CONF_U_INVERTER_ID])
    cg.add(parent.register_listener(var))

    if conf := config.get(CONF_OPERATING_MODE):
        sens = await text_sensor.new_text_sensor(conf)
        cg.add(var.set_operating_mode_tsensor(sens))
