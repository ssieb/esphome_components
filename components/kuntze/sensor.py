import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, modbus
from esphome.const import CONF_ID, UNIT_EMPTY, ICON_EMPTY, DEVICE_CLASS_EMPTY, CONF_TEMPERATURE, ICON_THERMOMETER, UNIT_CELSIUS, DEVICE_CLASS_TEMPERATURE

AUTO_LOAD = ['modbus']

kuntze_ns = cg.esphome_ns.namespace('kuntze')
Kuntze = kuntze_ns.class_('Kuntze', cg.PollingComponent, modbus.ModbusDevice)

CONF_PH = "ph"
CONF_DIS1 = "dis1"
CONF_DIS2 = "dis2"
CONF_REDOX = "redox"
CONF_EC = "ec"
CONF_OCI = "oci"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(Kuntze),
    cv.Optional(CONF_PH): sensor.sensor_schema(UNIT_EMPTY, ICON_EMPTY, 1, DEVICE_CLASS_EMPTY),
    cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(UNIT_CELSIUS, ICON_THERMOMETER, 1, DEVICE_CLASS_TEMPERATURE),
    cv.Optional(CONF_DIS1): sensor.sensor_schema(UNIT_EMPTY, ICON_EMPTY, 1, DEVICE_CLASS_EMPTY),
    cv.Optional(CONF_DIS2): sensor.sensor_schema(UNIT_EMPTY, ICON_EMPTY, 1, DEVICE_CLASS_EMPTY),
    cv.Optional(CONF_REDOX): sensor.sensor_schema(UNIT_EMPTY, ICON_EMPTY, 1, DEVICE_CLASS_EMPTY),
    cv.Optional(CONF_EC): sensor.sensor_schema(UNIT_EMPTY, ICON_EMPTY, 1, DEVICE_CLASS_EMPTY),
    cv.Optional(CONF_OCI): sensor.sensor_schema(UNIT_EMPTY, ICON_EMPTY, 1, DEVICE_CLASS_EMPTY),
}).extend(cv.polling_component_schema('60s')).extend(modbus.modbus_device_schema(0x01))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await modbus.register_modbus_device(var, config)

    if CONF_PH in config:
        conf = config[CONF_PH]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_ph_sensor(sens))
    if CONF_TEMPERATURE in config:
        conf = config[CONF_TEMPERATURE]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_temperature_sensor(sens))
    if CONF_DIS1 in config:
        conf = config[CONF_DIS1]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_dis1_sensor(sens))
    if CONF_DIS2 in config:
        conf = config[CONF_DIS2]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_dis2_sensor(sens))
    if CONF_REDOX in config:
        conf = config[CONF_REDOX]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_redox_sensor(sens))
    if CONF_EC in config:
        conf = config[CONF_EC]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_ec_sensor(sens))
    if CONF_OCI in config:
        conf = config[CONF_OCI]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_oci_sensor(sens))
