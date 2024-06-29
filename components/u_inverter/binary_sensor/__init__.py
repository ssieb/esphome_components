import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_PROBLEM,
    ENTITY_CATEGORY_DIAGNOSTIC,
)
from .. import (
    u_inverter_ns,
    UInverter,
    CONF_U_INVERTER_ID,
)

UInverterBSensor = u_inverter_ns.class_("UInverterBSensor", cg.Component)

CONF_LOAD_ENERGY = "load_energy"
CONF_HAS_OUTPUT = "has_output"
CONF_LOW_BATTERY = "low_battery"
CONF_BATTERY_CONNECTED = "battery_connected"
CONF_OUTPUT_OVERLOAD = "output_overload"
CONF_OVER_TEMPERATURE = "over_temperature"
CONF_PV_POWER_LOW = "pv_power_low"
CONF_INPUT_TOO_HIGH = "input_too_high"
CONF_BATTERY_TOO_HIGH = "battery_too_high"
CONF_FAN_SPEED_ERROR = "fan_speed_error"

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(UInverterBSensor),
        cv.GenerateID(CONF_U_INVERTER_ID): cv.use_id(UInverter),
        cv.Optional(CONF_LOAD_ENERGY): binary_sensor.binary_sensor_schema(),
        cv.Optional(CONF_HAS_OUTPUT): binary_sensor.binary_sensor_schema(),
        cv.Optional(CONF_LOW_BATTERY): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_PROBLEM,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_BATTERY_CONNECTED): binary_sensor.binary_sensor_schema(),
        cv.Optional(CONF_OUTPUT_OVERLOAD): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_PROBLEM,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_OVER_TEMPERATURE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_PROBLEM,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_PV_POWER_LOW): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_PROBLEM,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_INPUT_TOO_HIGH): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_PROBLEM,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_BATTERY_TOO_HIGH): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_PROBLEM,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_FAN_SPEED_ERROR): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_PROBLEM,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    parent = await cg.get_variable(config[CONF_U_INVERTER_ID])
    cg.add(parent.register_listener(var))

    if conf := config.get(CONF_LOAD_ENERGY):
        sens = await binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_load_energy_bsensor(sens))
    if conf := config.get(CONF_HAS_OUTPUT):
        sens = await binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_has_output_bsensor(sens))
    if conf := config.get(CONF_LOW_BATTERY):
        sens = await binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_low_battery_bsensor(sens))
    if conf := config.get(CONF_BATTERY_CONNECTED):
        sens = await binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_battery_connected_bsensor(sens))
    if conf := config.get(CONF_OUTPUT_OVERLOAD):
        sens = await binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_output_overload_bsensor(sens))
    if conf := config.get(CONF_OVER_TEMPERATURE):
        sens = await binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_over_temperature_bsensor(sens))
    if conf := config.get(CONF_PV_POWER_LOW):
        sens = await binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_pv_power_low_bsensor(sens))
    if conf := config.get(CONF_INPUT_TOO_HIGH):
        sens = await binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_input_too_high_bsensor(sens))
    if conf := config.get(CONF_BATTERY_TOO_HIGH):
        sens = await binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_battery_too_high_bsensor(sens))
    if conf := config.get(CONF_FAN_SPEED_ERROR):
        sens = await binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_fan_speed_error_bsensor(sens))
