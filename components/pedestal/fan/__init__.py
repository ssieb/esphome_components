import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import fan, remote_base, sensor
from esphome.const import CONF_OUTPUT_ID

pedestal_ns = cg.esphome_ns.namespace("pedestal")
PedestalFan = pedestal_ns.class_(
    "PedestalFan",
    cg.Component,
    fan.Fan,
    remote_base.RemoteTransmittable,
)

CONF_OSC_PIN = "osc_pin"
CONF_PULSE = "pulse"

CONFIG_SCHEMA = fan.FAN_SCHEMA.extend(
    {
        cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(PedestalFan),
        cv.Required(CONF_PULSE): cv.use_id(sensor.Sensor),
        cv.Required(CONF_OSC_PIN): pins.gpio_input_pin_schema,
    }
).extend(cv.COMPONENT_SCHEMA).extend(remote_base.REMOTE_TRANSMITTABLE_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await cg.register_component(var, config)
    await fan.register_fan(var, config)
    await remote_base.register_transmittable(var, config)
    sens = await cg.get_variable(config[CONF_PULSE])
    cg.add(var.set_pulse_sensor(sens))
    pin = await cg.gpio_pin_expression(config[CONF_OSC_PIN])
    cg.add(var.set_osc_pin(pin))
