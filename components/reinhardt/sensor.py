import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import uart
from esphome.components import sensor
from esphome.const import (
    CONF_ALTITUDE,
    CONF_HUMIDITY,
    CONF_ID,
    CONF_LATITUDE,
    CONF_LIGHT,
    CONF_LONGITUDE,
    CONF_PRESSURE,
    CONF_SATELLITES,
    CONF_SENSORS,
    CONF_SPEED,
    CONF_TEMPERATURE,
    CONF_WIND_DIRECTION_DEGREES,
    CONF_WIND_SPEED,
    DEVICE_CLASS_ATMOSPHERIC_PRESSURE,
    DEVICE_CLASS_HUMIDITY,
    DEVICE_CLASS_ILLUMINANCE,
    DEVICE_CLASS_PRECIPITATION,
    DEVICE_CLASS_PRESSURE,
    DEVICE_CLASS_SPEED,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_WIND_DIRECTION,
    DEVICE_CLASS_WIND_SPEED,
    ICON_BRIGHTNESS_5,
    ICON_GAUGE,
    ICON_SIGN_DIRECTION,
    ICON_THERMOMETER,
    ICON_WATER_PERCENT,
    ICON_WEATHER_WINDY,
    STATE_CLASS_MEASUREMENT,
    UNIT_CELSIUS,
    UNIT_DEGREES,
    UNIT_HECTOPASCAL,
    UNIT_KILOMETER_PER_HOUR,
    UNIT_LUX,
    UNIT_METER,
    UNIT_MILLIMETER,
    UNIT_PERCENT,
)

DEPENDENCIES = ['uart']

reinhardt_ns = cg.esphome_ns.namespace('reinhardt')

SensorType = reinhardt_ns.enum("SensorType")
Reinhardt = reinhardt_ns.class_('Reinhardt', cg.Component, uart.UARTDevice)

CONF_INDOOR_TEMPERATURE = "indoor_temperature"
CONF_WIND_DIRECTION = "wind_direction"
CONF_PREVAILING_WIND_DIRECTION = "prevailing_wind_direction"
CONF_PEAK_WIND_SPEED = "peak_wind_speed"
CONF_AVERAGE_WIND_SPEED = "average_wind_speed"
CONF_RAINFALL = "rainfall"
CONF_RAINFALL_OVER_INTERVAL = "rainfall_over_interval"
CONF_SOLAR_RADIATION = "solar_radiation"
CONF_UV_RADIATION = "uv_radiation"
CONF_BAROMETER = "barometer"
CONF_DEWPOINT = "dewpoint"
CONF_WINDCHILL = "windchill"
CONF_GPS_ALTITUDE = "gps_altitude"

ICON_ALTIMETER = "mdi:altimeter"
ICON_COMPASS = "mdi:compass"
ICON_LATITUDE = "mdi:latitude"
ICON_LONGITUDE = "mdi:longitude"
ICON_SATELLITE = "mdi:satellite-variant"
ICON_SPEEDOMETER = "mdi:speedometer"

UNIT_RADIATION = "W/m²"
UNIT_UV_RADIATION = "W/m²"

CONFIG_SCHEMA = uart.UART_DEVICE_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(Reinhardt),
        cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            device_class=DEVICE_CLASS_TEMPERATURE,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_HUMIDITY): sensor.sensor_schema(
            unit_of_measurement=UNIT_PERCENT,
            icon=ICON_WATER_PERCENT,
            device_class=DEVICE_CLASS_HUMIDITY,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_INDOOR_TEMPERATURE): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            device_class=DEVICE_CLASS_TEMPERATURE,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_PRESSURE): sensor.sensor_schema(
            unit_of_measurement=UNIT_HECTOPASCAL,
            icon=ICON_GAUGE,
            device_class=DEVICE_CLASS_ATMOSPHERIC_PRESSURE,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_WIND_DIRECTION): sensor.sensor_schema(
            unit_of_measurement=UNIT_DEGREES,
            icon=ICON_SIGN_DIRECTION,
            device_class=DEVICE_CLASS_WIND_DIRECTION,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_PREVAILING_WIND_DIRECTION): sensor.sensor_schema(
            unit_of_measurement=UNIT_DEGREES,
            icon=ICON_SIGN_DIRECTION,
            device_class=DEVICE_CLASS_WIND_DIRECTION,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_WIND_SPEED): sensor.sensor_schema(
            unit_of_measurement=UNIT_KILOMETER_PER_HOUR,
            icon=ICON_WEATHER_WINDY,
            device_class=DEVICE_CLASS_WIND_SPEED,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_PEAK_WIND_SPEED): sensor.sensor_schema(
            unit_of_measurement=UNIT_KILOMETER_PER_HOUR,
            icon=ICON_WEATHER_WINDY,
            device_class=DEVICE_CLASS_WIND_SPEED,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_AVERAGE_WIND_SPEED): sensor.sensor_schema(
            unit_of_measurement=UNIT_KILOMETER_PER_HOUR,
            icon=ICON_WEATHER_WINDY,
            device_class=DEVICE_CLASS_WIND_SPEED,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_RAINFALL): sensor.sensor_schema(
            unit_of_measurement=UNIT_MILLIMETER,
            device_class=DEVICE_CLASS_PRECIPITATION,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_RAINFALL_OVER_INTERVAL): sensor.sensor_schema(
            unit_of_measurement=UNIT_MILLIMETER,
            device_class=DEVICE_CLASS_PRECIPITATION,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_SOLAR_RADIATION): sensor.sensor_schema(
            unit_of_measurement=UNIT_RADIATION,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_UV_RADIATION): sensor.sensor_schema(
            unit_of_measurement=UNIT_UV_RADIATION,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_LIGHT): sensor.sensor_schema(
            unit_of_measurement=UNIT_LUX,
            icon=ICON_BRIGHTNESS_5,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_ILLUMINANCE,
        ),
        cv.Optional(CONF_BAROMETER): sensor.sensor_schema(
            unit_of_measurement=UNIT_HECTOPASCAL,
            icon=ICON_GAUGE,
            device_class=DEVICE_CLASS_ATMOSPHERIC_PRESSURE,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_DEWPOINT): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            device_class=DEVICE_CLASS_TEMPERATURE,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_WINDCHILL): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            device_class=DEVICE_CLASS_TEMPERATURE,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_ALTITUDE): sensor.sensor_schema(
            unit_of_measurement=UNIT_METER,
            icon=ICON_ALTIMETER,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_GPS_ALTITUDE): sensor.sensor_schema(
            unit_of_measurement=UNIT_METER,
            icon=ICON_ALTIMETER,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_LATITUDE): sensor.sensor_schema(
            unit_of_measurement=UNIT_DEGREES,
            icon=ICON_LATITUDE,
            accuracy_decimals=6,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_LONGITUDE): sensor.sensor_schema(
            unit_of_measurement=UNIT_DEGREES,
            icon=ICON_LONGITUDE,
            accuracy_decimals=6,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_SPEED): sensor.sensor_schema(
            unit_of_measurement=UNIT_KILOMETER_PER_HOUR,
            icon=ICON_SPEEDOMETER,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_SPEED,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_SATELLITES): sensor.sensor_schema(
            icon=ICON_SATELLITE,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
    }
)

SENSOR_TYPE_MAP = {
    CONF_TEMPERATURE: SensorType.RS_TEMPERATURE,
    CONF_HUMIDITY: SensorType.RS_HUMIDITY,
    CONF_INDOOR_TEMPERATURE: SensorType.RS_IN_TEMPERATURE,
    CONF_PRESSURE: SensorType.RS_PRESSURE,
    CONF_WIND_DIRECTION: SensorType.RS_DIRECTION,
    CONF_PREVAILING_WIND_DIRECTION: SensorType.RS_PREVAILING,
    CONF_WIND_SPEED: SensorType.RS_WINDSPEED,
    CONF_PEAK_WIND_SPEED: SensorType.RS_PEAKSPEED,
    CONF_AVERAGE_WIND_SPEED: SensorType.RS_AVGSPEED,
    CONF_RAINFALL: SensorType.RS_RAIN,
    CONF_RAINFALL_OVER_INTERVAL: SensorType.RS_RAIN_INTERVAL,
    CONF_SOLAR_RADIATION: SensorType.RS_RADIATION,
    CONF_UV_RADIATION: SensorType.RS_UVRADIATION,
    CONF_LIGHT: SensorType.RS_BRIGHTNESS,
    CONF_BAROMETER: SensorType.RS_BAROMETER,
    CONF_DEWPOINT: SensorType.RS_DEWPOINT,
    CONF_WINDCHILL: SensorType.RS_WINDCHILL,
    CONF_ALTITUDE: SensorType.RS_ALTITUDE,
    CONF_GPS_ALTITUDE: SensorType.RS_GPS_ALTITUDE,
    CONF_LATITUDE: SensorType.RS_GPS_LATITUDE,
    CONF_LONGITUDE: SensorType.RS_GPS_LONGITUDE,
    CONF_SPEED: SensorType.RS_GPS_SPEED,
    CONF_SATELLITES: SensorType.RS_GPS_SATELLITES,
}

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
    for key, stype in SENSOR_TYPE_MAP.items():
        if conf := config.get(key):
          sens = await sensor.new_sensor(conf)
          cg.add(var.add_sensor(stype, sens))
