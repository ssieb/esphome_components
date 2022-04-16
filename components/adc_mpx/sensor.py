import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import sensor, voltage_sampler
from esphome.const import (
    CONF_ID,
    CONF_PIN,
    CONF_SENSOR,
    DEVICE_CLASS_VOLTAGE,
    ICON_EMPTY,
    UNIT_VOLT,
)

adc_mpx_ns = cg.esphome_ns.namespace("adc_mpx")
ADCMPXSensor = adc_mpx_ns.class_(
    "ADCMPXSensor", sensor.Sensor, cg.PollingComponent
)

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        ADCMPXSensor,
        unit_of_measurement=UNIT_VOLT,
        icon=ICON_EMPTY,
        accuracy_decimals=2,
        device_class=DEVICE_CLASS_VOLTAGE
    ).extend(
        {
            cv.Required(CONF_SENSOR): cv.use_id(voltage_sampler.VoltageSampler),
            cv.Required(CONF_PIN): pins.gpio_output_pin_schema
        }
    ).extend(cv.polling_component_schema("60s"))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)

    sens = await cg.get_variable(config[CONF_SENSOR])
    cg.add(var.set_source(sens))
    pin = await cg.gpio_pin_expression(config[CONF_PIN])
    cg.add(var.set_pin(pin))
