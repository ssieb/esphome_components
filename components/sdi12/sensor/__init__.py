import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ADDRESS,
    CONF_ID,
    CONF_INDEX,
    CONF_SENSORS,
)
from .. import (
    sdi12_ns,
    SDI12,
    CONF_SDI12_ID,
)

SDI12Sensor = sdi12_ns.class_("SDI12Sensor", cg.PollingComponent)

CONFIG_SCHEMA = cv.polling_component_schema('60s').extend(
    {
        cv.GenerateID(): cv.declare_id(SDI12Sensor),
        cv.GenerateID(CONF_SDI12_ID): cv.use_id(SDI12),
        cv.Required(CONF_ADDRESS): cv.int_range(0, 61),
        cv.Optional(CONF_SENSORS): cv.ensure_list(
            sensor.sensor_schema().extend(
                {
                    cv.GenerateID(): cv.declare_id(sensor.Sensor),
                    cv.Required(CONF_INDEX): cv.int_range(1, 9),
                }
            )
        ),
    }
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    sdi12 = await cg.get_variable(config[CONF_SDI12_ID])
    cg.add(sdi12.register_listener(var))
    cg.add(var.set_parent(sdi12))
    cg.add(var.set_address(config[CONF_ADDRESS]))
    for conf in config[CONF_SENSORS]:
        sens = await sensor.new_sensor(conf)
        cg.add(var.add_sensor(conf[CONF_INDEX] - 1, sens))
