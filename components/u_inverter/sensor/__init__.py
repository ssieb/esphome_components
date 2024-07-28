import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_ENERGY,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_VOLTAGE,
    ICON_FLASH,
    ICON_CURRENT_AC,
    ICON_PERCENT,
    ICON_THERMOMETER,
    STATE_CLASS_MEASUREMENT,
    UNIT_AMPERE,
    UNIT_CELSIUS,
    UNIT_HERTZ,
    UNIT_KILOWATT_HOURS,
    UNIT_PERCENT,
    UNIT_VOLT,
    UNIT_VOLT_AMPS,
    UNIT_WATT,
)
from .. import (
    u_inverter_ns,
    UInverter,
    CONF_U_INVERTER_ID,
)

UInverterSensor = u_inverter_ns.class_("UInverterSensor", cg.Component)

UNIT_MEGAWATT_HOURS = "MWh"

CONF_FAULT_CODE = "fault_code"
CONF_MAINS_VOLTAGE = "mains_voltage"
CONF_MAINS_FREQUENCY = "mains_frequency"
CONF_MAINS_LOSSV_HIGH = "mains_lossv_high"
CONF_MAINS_LOSSV_LOW = "mains_lossv_low"
CONF_MAINS_LOSSF_HIGH = "mains_lossf_high"
CONF_MAINS_LOSSF_LOW = "mains_lossf_low"

CONF_OUTPUT_VOLTAGE = "output_voltage"
CONF_OUTPUT_FREQUENCY = "output_frequency"
CONF_OUTPUT_REACTIVE_POWER = "output_reactive_power"
CONF_OUTPUT_ACTIVE_POWER = "output_active_power"
CONF_LOAD_PERCENT = "load_percent"
CONF_OUTPUT_DC = "output_dc"

CONF_NUMBER_CELLS = "number_cells"
CONF_BATTERY_VOLTAGE = "battery_voltage"
CONF_BATTERY_CAPACITY = "battery_capacity"
CONF_BATTERY_CHARGE_CURRENT = "battery_charge_current"
CONF_BATTERY_DISCHARGE_CURRENT = "battery_discharge_current"
CONF_INV_BUS_VOLTAGE = "inv_bus_voltage"

CONF_PV_VOLTAGE = "pv_voltage"
CONF_PV_CURRENT = "pv_current"
CONF_PV_POWER = "pv_power"

CONF_PVB_VOLTAGE = "pvb_voltage"
CONF_PVB_CURRENT = "pvb_current"
CONF_PVB_POWER = "pvb_power"

CONF_PV_INPUT_TEMP = "pv_input_temp"
CONF_INVERTER_TEMP = "inverter_temp"
CONF_BOOST_TEMP = "boost_temp"
CONF_TRANSFORMER_TEMP = "transformer_temp"
CONF_HOTSPOT_TEMP = "hotspot_temp"
CONF_FAN1_SPEED = "fan1_speed"
CONF_FAN2_SPEED = "fan2_speed"

CONF_MAX_TOTAL_CHARGING_CURRENT = "max_total_charging_current"
CONF_MAX_GRID_CHARGING_CURRENT = "max_grid_charging_current"
CONF_OUTPUT_SET_VOLTAGE = "output_set_voltage"
CONF_TARGET_CHARGING_VOLTAGE = "target_charging_voltage"
CONF_BATTERY_FLOAT_VOLTAGE = "battery_float_voltage"
CONF_BATTERY_POWER_OFF_VOLTAGE = "battery_power_off_voltage"

CONF_DAILY_ENERGY = "daily_energy"
CONF_MONTHLY_ENERGY = "monthly_energy"
CONF_ANNUAL_ENERGY = "annual_energy"
CONF_TOTAL_ENERGY = "total_energy"

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(UInverterSensor),
        cv.GenerateID(CONF_U_INVERTER_ID): cv.use_id(UInverter),

        cv.Optional(CONF_FAULT_CODE): sensor.sensor_schema(
            accuracy_decimals=0,
        ),

        cv.Optional(CONF_MAINS_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_VOLTAGE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_MAINS_FREQUENCY): sensor.sensor_schema(
            unit_of_measurement=UNIT_HERTZ,
            icon=ICON_CURRENT_AC,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_MAINS_LOSSV_HIGH): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_VOLTAGE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_MAINS_LOSSV_LOW): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_VOLTAGE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_MAINS_LOSSF_HIGH): sensor.sensor_schema(
            unit_of_measurement=UNIT_HERTZ,
            icon=ICON_CURRENT_AC,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_MAINS_LOSSF_LOW): sensor.sensor_schema(
            unit_of_measurement=UNIT_HERTZ,
            icon=ICON_CURRENT_AC,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),

        cv.Optional(CONF_OUTPUT_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_VOLTAGE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_OUTPUT_FREQUENCY): sensor.sensor_schema(
            unit_of_measurement=UNIT_HERTZ,
            icon=ICON_CURRENT_AC,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_OUTPUT_REACTIVE_POWER): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT_AMPS,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_OUTPUT_ACTIVE_POWER): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_LOAD_PERCENT): sensor.sensor_schema(
            unit_of_measurement=UNIT_PERCENT,
            icon=ICON_PERCENT,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_OUTPUT_DC): sensor.sensor_schema(
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),

        cv.Optional(CONF_NUMBER_CELLS): sensor.sensor_schema(
            accuracy_decimals=0,
        ),
        cv.Optional(CONF_BATTERY_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_VOLTAGE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_BATTERY_CAPACITY): sensor.sensor_schema(
            unit_of_measurement=UNIT_PERCENT,
            icon=ICON_PERCENT,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_BATTERY_CHARGE_CURRENT): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE,
            icon=ICON_CURRENT_AC,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_CURRENT,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_BATTERY_DISCHARGE_CURRENT): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE,
            icon=ICON_CURRENT_AC,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_CURRENT,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_INV_BUS_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_VOLTAGE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),

        cv.Optional(CONF_PV_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_VOLTAGE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_PV_CURRENT): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE,
            icon=ICON_CURRENT_AC,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_CURRENT,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_PV_POWER): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
        ),

        cv.Optional(CONF_PVB_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_VOLTAGE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_PVB_CURRENT): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE,
            icon=ICON_CURRENT_AC,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_CURRENT,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_PVB_POWER): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
        ),

        cv.Optional(CONF_PV_INPUT_TEMP): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
            device_class=DEVICE_CLASS_TEMPERATURE,
        ),
        cv.Optional(CONF_INVERTER_TEMP): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
            device_class=DEVICE_CLASS_TEMPERATURE,
        ),
        cv.Optional(CONF_BOOST_TEMP): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
            device_class=DEVICE_CLASS_TEMPERATURE,
        ),
        cv.Optional(CONF_TRANSFORMER_TEMP): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
            device_class=DEVICE_CLASS_TEMPERATURE,
        ),
        cv.Optional(CONF_HOTSPOT_TEMP): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
            device_class=DEVICE_CLASS_TEMPERATURE,
        ),
        cv.Optional(CONF_FAN1_SPEED): sensor.sensor_schema(
            unit_of_measurement=UNIT_PERCENT,
            icon=ICON_PERCENT,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_FAN2_SPEED): sensor.sensor_schema(
            unit_of_measurement=UNIT_PERCENT,
            icon=ICON_PERCENT,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),

        cv.Optional(CONF_MAX_TOTAL_CHARGING_CURRENT): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE,
            icon=ICON_CURRENT_AC,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_CURRENT,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_MAX_GRID_CHARGING_CURRENT): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE,
            icon=ICON_CURRENT_AC,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_CURRENT,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_OUTPUT_SET_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_VOLTAGE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_TARGET_CHARGING_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_VOLTAGE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_BATTERY_FLOAT_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_VOLTAGE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_BATTERY_POWER_OFF_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_VOLTAGE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),

        cv.Optional(CONF_DAILY_ENERGY): sensor.sensor_schema(
            unit_of_measurement=UNIT_KILOWATT_HOURS,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_ENERGY,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_MONTHLY_ENERGY): sensor.sensor_schema(
            unit_of_measurement=UNIT_KILOWATT_HOURS,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_ENERGY,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_ANNUAL_ENERGY): sensor.sensor_schema(
            unit_of_measurement=UNIT_MEGAWATT_HOURS,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_ENERGY,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_TOTAL_ENERGY): sensor.sensor_schema(
            unit_of_measurement=UNIT_MEGAWATT_HOURS,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_ENERGY,
            state_class=STATE_CLASS_MEASUREMENT,
        ),

    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    parent = await cg.get_variable(config[CONF_U_INVERTER_ID])
    cg.add(parent.register_listener(var))

    if conf := config.get(CONF_FAULT_CODE):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_fault_code_sensor(sens))

    if conf := config.get(CONF_MAINS_VOLTAGE):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_mains_voltage_sensor(sens))
    if conf := config.get(CONF_MAINS_FREQUENCY):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_mains_frequency_sensor(sens))
    if conf := config.get(CONF_MAINS_LOSSV_HIGH):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_mains_lossv_high_sensor(sens))
    if conf := config.get(CONF_MAINS_LOSSV_LOW):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_mains_lossv_low_sensor(sens))
    if conf := config.get(CONF_MAINS_LOSSF_HIGH):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_mains_lossf_high_sensor(sens))
    if conf := config.get(CONF_MAINS_LOSSF_LOW):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_mains_lossf_low_sensor(sens))

    if conf := config.get(CONF_OUTPUT_VOLTAGE):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_output_voltage_sensor(sens))
    if conf := config.get(CONF_OUTPUT_FREQUENCY):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_output_frequency_sensor(sens))
    if conf := config.get(CONF_OUTPUT_REACTIVE_POWER):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_output_reactive_power_sensor(sens))
    if conf := config.get(CONF_OUTPUT_ACTIVE_POWER):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_output_active_power_sensor(sens))
    if conf := config.get(CONF_LOAD_PERCENT):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_load_percent_sensor(sens))
    if conf := config.get(CONF_OUTPUT_DC):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_output_dc_sensor(sens))

    if conf := config.get(CONF_NUMBER_CELLS):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_number_cells_sensor(sens))
    if conf := config.get(CONF_BATTERY_VOLTAGE):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_battery_voltage_sensor(sens))
    if conf := config.get(CONF_BATTERY_CAPACITY):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_battery_capacity_sensor(sens))
    if conf := config.get(CONF_BATTERY_CHARGE_CURRENT):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_battery_charge_current_sensor(sens))
    if conf := config.get(CONF_BATTERY_DISCHARGE_CURRENT):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_battery_discharge_current_sensor(sens))
    if conf := config.get(CONF_INV_BUS_VOLTAGE):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_inv_bus_voltage_sensor(sens))

    if conf := config.get(CONF_PV_VOLTAGE):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_pv_voltage_sensor(sens))
    if conf := config.get(CONF_PV_CURRENT):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_pv_current_sensor(sens))
    if conf := config.get(CONF_PV_POWER):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_pv_power_sensor(sens))

    if conf := config.get(CONF_PVB_VOLTAGE):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_pvb_voltage_sensor(sens))
    if conf := config.get(CONF_PVB_CURRENT):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_pvb_current_sensor(sens))
    if conf := config.get(CONF_PVB_POWER):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_pvb_power_sensor(sens))

    if conf := config.get(CONF_PV_INPUT_TEMP):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_pv_input_temp_sensor(sens))
    if conf := config.get(CONF_INVERTER_TEMP):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_inverter_temp_sensor(sens))
    if conf := config.get(CONF_BOOST_TEMP):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_boost_temp_sensor(sens))
    if conf := config.get(CONF_TRANSFORMER_TEMP):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_transformer_temp_sensor(sens))
    if conf := config.get(CONF_HOTSPOT_TEMP):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_hotspot_temp_sensor(sens))
    if conf := config.get(CONF_FAN1_SPEED):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_fan1_speed_sensor(sens))
    if conf := config.get(CONF_FAN2_SPEED):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_fan2_speed_sensor(sens))

    if conf := config.get(CONF_MAX_TOTAL_CHARGING_CURRENT):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_max_total_charging_current_sensor(sens))
    if conf := config.get(CONF_MAX_GRID_CHARGING_CURRENT):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_max_grid_charging_current_sensor(sens))
    if conf := config.get(CONF_OUTPUT_SET_VOLTAGE):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_output_set_voltage_sensor(sens))
    if conf := config.get(CONF_TARGET_CHARGING_VOLTAGE):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_target_charging_voltage_sensor(sens))
    if conf := config.get(CONF_BATTERY_FLOAT_VOLTAGE):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_battery_float_voltage_sensor(sens))
    if conf := config.get(CONF_BATTERY_POWER_OFF_VOLTAGE):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_battery_power_off_voltage_sensor(sens))

    if conf := config.get(CONF_DAILY_ENERGY):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_daily_energy_sensor(sens))
    if conf := config.get(CONF_MONTHLY_ENERGY):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_monthly_energy_sensor(sens))
    if conf := config.get(CONF_ANNUAL_ENERGY):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_annual_energy_sensor(sens))
    if conf := config.get(CONF_TOTAL_ENERGY):
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_total_energy_sensor(sens))
