import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import uart, binary_sensor, sensor, text_sensor
from esphome.const import (
    CONF_ID,
    CONF_HUMIDITY,
    CONF_TEMPERATURE,
    CONF_TIME,
    CONF_WIND_SPEED,
    DEVICE_CLASS_EMPTY,
    DEVICE_CLASS_HUMIDITY,
    DEVICE_CLASS_ILLUMINANCE,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_TIMESTAMP,
    ICON_BRIGHTNESS_5,
    ICON_EMPTY,
    ICON_PERCENT,
    ICON_THERMOMETER,
    UNIT_CELSIUS,
    UNIT_EMPTY,
    UNIT_LUX,
    UNIT_PERCENT,
)

MULTI_CONF = True
DEPENDENCIES = ['uart']
AUTO_LOAD = ['binary_sensor', 'sensor', 'text_sensor']

clima_ns = cg.esphome_ns.namespace('clima')

Clima = clima_ns.class_('Clima', cg.Component, uart.UARTDevice)

CONF_BRIGHTNESS_EAST = "brightness_east"
CONF_BRIGHTNESS_WEST = "brightness_west"
CONF_BRIGHTNESS_SOUTH = "brightness_south"
CONF_TWILIGHT = "twilight"
CONF_PRECIPITATION = "precipitation"
CONF_LOW_VOLTAGE = "low_voltage"
CONF_NO_TIME_SYNC = "no_time_sync"
CONF_HEATING = "heating"
CONF_TIME_SYNCING = "time_syncing"

UNIT_METER_PER_SECOND = "m/s"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(Clima),
    cv.Optional(CONF_TIME): text_sensor.text_sensor_schema(text_sensor.TextSensor),
    cv.Optional(CONF_BRIGHTNESS_EAST): sensor.sensor_schema(
        unit_of_measurement=UNIT_LUX,
        icon=ICON_BRIGHTNESS_5,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_ILLUMINANCE,
    ),
    cv.Optional(CONF_BRIGHTNESS_WEST): sensor.sensor_schema(
        unit_of_measurement=UNIT_LUX,
        icon=ICON_BRIGHTNESS_5,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_ILLUMINANCE,
    ),
    cv.Optional(CONF_BRIGHTNESS_SOUTH): sensor.sensor_schema(
        unit_of_measurement=UNIT_LUX,
        icon=ICON_BRIGHTNESS_5,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_ILLUMINANCE,
    ),
    cv.Optional(CONF_TWILIGHT): sensor.sensor_schema(
        unit_of_measurement=UNIT_LUX,
        icon=ICON_BRIGHTNESS_5,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_ILLUMINANCE,
    ),
    cv.Optional(CONF_HUMIDITY): sensor.sensor_schema(
        unit_of_measurement=UNIT_PERCENT,
        icon=ICON_PERCENT,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_HUMIDITY
    ),
    cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
        unit_of_measurement=UNIT_CELSIUS,
        icon=ICON_THERMOMETER,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_TEMPERATURE
    ),
    cv.Optional(CONF_PRECIPITATION): binary_sensor.binary_sensor_schema(),
    cv.Optional(CONF_WIND_SPEED): sensor.sensor_schema(
        unit_of_measurement=UNIT_METER_PER_SECOND,
        icon=ICON_EMPTY,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_EMPTY
    ),
    cv.Optional(CONF_LOW_VOLTAGE): binary_sensor.binary_sensor_schema(),
    cv.Optional(CONF_NO_TIME_SYNC): binary_sensor.binary_sensor_schema(),
    cv.Optional(CONF_HEATING): binary_sensor.binary_sensor_schema(),
    cv.Optional(CONF_TIME_SYNCING): binary_sensor.binary_sensor_schema(),
}).extend(uart.UART_DEVICE_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    if CONF_TIME in config:
        sens = await text_sensor.new_text_sensor(config[CONF_TIME])
        cg.add(var.set_time_sensor(sens))

    if CONF_BRIGHTNESS_EAST in config:
        sens = await sensor.new_sensor(config[CONF_BRIGHTNESS_EAST])
        cg.add(var.set_brightness_east_sensor(sens))

    if CONF_BRIGHTNESS_WEST in config:
        sens = await sensor.new_sensor(config[CONF_BRIGHTNESS_WEST])
        cg.add(var.set_brightness_west_sensor(sens))

    if CONF_BRIGHTNESS_SOUTH in config:
        sens = await sensor.new_sensor(config[CONF_BRIGHTNESS_SOUTH])
        cg.add(var.set_brightness_south_sensor(sens))

    if CONF_TWILIGHT in config:
        sens = await sensor.new_sensor(config[CONF_TWILIGHT])
        cg.add(var.set_twilight_sensor(sens))

    if CONF_HUMIDITY in config:
        sens = await sensor.new_sensor(config[CONF_HUMIDITY])
        cg.add(var.set_humidity_sensor(sens))

    if CONF_TEMPERATURE in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATURE])
        cg.add(var.set_temperature_sensor(sens))

    if CONF_PRECIPITATION in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_PRECIPITATION])
        cg.add(var.set_precipitation_sensor(sens))

    if CONF_WIND_SPEED in config:
        sens = await sensor.new_sensor(config[CONF_WIND_SPEED])
        cg.add(var.set_windspeed_sensor(sens))

    if CONF_LOW_VOLTAGE in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_LOW_VOLTAGE])
        cg.add(var.set_low_voltage_sensor(sens))

    if CONF_NO_TIME_SYNC in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_NO_TIME_SYNC])
        cg.add(var.set_no_time_sync_sensor(sens))

    if CONF_HEATING in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_HEATING])
        cg.add(var.set_heating_sensor(sens))

    if CONF_TIME_SYNCING in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_TIME_SYNCING])
        cg.add(var.set_time_syncing_sensor(sens))
