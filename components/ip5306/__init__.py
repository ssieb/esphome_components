import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor, i2c, sensor
from esphome.const import CONF_ID, CONF_BATTERY_LEVEL, DEVICE_CLASS_VOLTAGE, ICON_PERCENT, UNIT_PERCENT

MULTI_CONF = True

AUTO_LOAD = [ "binary_sensor", "sensor" ]

ip5306_ns = cg.esphome_ns.namespace('ip5306')
IP5306 = ip5306_ns.class_('IP5306', i2c.I2CDevice, cg.Component)

CONF_CHARGER_CONNECTED = "charger_connected"
CONF_CHARGER_ACTIVE = "charger_active"
CONF_CHARGE_FULL = "charge_full"
CONF_POWER_BOOST_ON = "power_boost_on"
CONF_POWER_BOOST_SET = "power_boost_set"
CONF_POWER_VIN = "power_vin"
CONF_POWER_BTN = "power_btn"
CONF_POWER_BOOST_KEEP_ON = "power_boost_keep_on"
CONF_AUTO_BOOT_ON_LOAD = "auto_boot_on_load"
CONF_ENABLE_POWER_BTN = "enable_power_btn"
CONF_LOW_POWER_SHUTDOWN_TIME = "low_power_shutdown_time"

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(IP5306),
        cv.Optional(CONF_BATTERY_LEVEL): sensor.sensor_schema(
            unit_of_measurement=UNIT_PERCENT,
            icon=ICON_PERCENT,
            accuracy_decimals=0,
        ),
        cv.Optional(CONF_CHARGER_CONNECTED): binary_sensor.binary_sensor_schema(),
        cv.Optional(CONF_CHARGE_FULL): binary_sensor.binary_sensor_schema(),
        cv.Optional(CONF_CHARGER_ACTIVE): binary_sensor.binary_sensor_schema(),
        cv.Optional(CONF_POWER_BOOST_ON, default=True): cv.boolean,
        cv.Optional(CONF_POWER_BOOST_SET, default=True): cv.boolean,
        cv.Optional(CONF_POWER_VIN, default=True): cv.boolean,
        cv.Optional(CONF_POWER_BTN, default=True): cv.boolean,
        cv.Optional(CONF_POWER_BOOST_KEEP_ON, default=True): cv.boolean,
        cv.Optional(CONF_AUTO_BOOT_ON_LOAD, default=False): cv.boolean,
        cv.Optional(CONF_ENABLE_POWER_BTN, default=True): cv.boolean,
        cv.Optional(CONF_LOW_POWER_SHUTDOWN_TIME, default=64): cv.uint8_t,
    }
).extend(i2c.i2c_device_schema(0x75))

def keys_to_code(config, var, types):
    for key in types:
        if key in config:
            conf = config[key]
            cg.add(getattr(var, f"set_{key}")(conf))

IP5306_KEYS = {
    CONF_POWER_BOOST_ON,
    CONF_POWER_BOOST_SET,
    CONF_POWER_VIN,
    CONF_POWER_BTN,
    CONF_POWER_BOOST_KEEP_ON,
    CONF_AUTO_BOOT_ON_LOAD,
    CONF_ENABLE_POWER_BTN,
    CONF_LOW_POWER_SHUTDOWN_TIME
}

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    if CONF_BATTERY_LEVEL in config:
        sens = await sensor.new_sensor(config[CONF_BATTERY_LEVEL])
        cg.add(var.set_battery_level(sens))

    if CONF_CHARGER_CONNECTED in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_CHARGER_CONNECTED])
        cg.add(var.set_charger_connected(sens))

    if CONF_CHARGER_ACTIVE in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_CHARGER_ACTIVE])
        cg.add(var.set_charger_active(sens))

    if CONF_CHARGE_FULL in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_CHARGE_FULL])
        cg.add(var.set_charge_full(sens))

    keys_to_code(config, var, IP5306_KEYS)
