# Support for a certain unknown inverter

A configured 2400 baud uart component is required.

All sensors and binary sensors are optional.

Example:
```yaml
u_inverter:

sensor:
  - platform: u_inverter
    fault_code:
      name: Fault Code
    mains_voltage:
      name: Mains Voltage
    mains_frequency:
      name: Mains Frequency
    mains_lossv_high:
      name: Mains Loss Voltage High
    mains_lossv_low:
      name: Mains Loss Voltage Low
    mains_lossf_high:
      name: Mains Loss Frequency High
    mains_lossf_low:
      name: Mains Loss Frequency Low
    mains_power:
      name: Mains Power

    output_voltage:
      name: Output Voltage
    output_frequency:
      name: Output Frequency
    output_reactive_power:
      name: Output Reactive Power
    output_active_power:
      name: Output Active Power
    load_percent:
      name: Load Percent
    output_dc:
      name: Output DC

    number_cells:
      name: Number of Cells
    battery_voltage:
      name: Battery Voltage
    battery_capacity:
      name: Battery Capacity
    battery_charge_current:
      name: Battery Charge Current
    battery_discharge_current:
      name: Battery Discharge Current
    inv_bus_voltage:
      name: Inverter Bus Voltage

    pv_voltage:
      name: PV Voltage
    pv_current:
      name: PV Current
    pv_power:
      name: PV Power

    pvb_voltage:
      name: PV 2 Voltage
    pvb_current:
      name: PV 2 Current
    pvb_power:
      name: PV 2 Power

    pv_input_temp:
      name: PV DC Input Temperature
    inverter_temp:
      name: Inverter Temperature
    boost_temp:
      name: Boost Temperature
    transformer_temp:
      name: Transformer Temperature
    hotspot_temp:
      name: Hot Spot Temperature
    fan1_speed:
      name: Fan 1 Speed Percentage
    fan2_speed:
      name: Fan 2 Speed Percentage

binary_sensor:
  - platform: u_inverter
    load_energy:
      name: Load Energized?
    has_output:
      name: Has Output
    low_battery:
      name: Low Battery
    battery_connected:
      name: Battery connected
    output_overload:
      name: Output Overload
    over_temperature:
      name: Over Temperature
    pv_power_low:
      name: PV Power Low
    input_too_high:
      name: Input Too High
    battery_too_high:
      name: Battery Too High
    fan_speed_error:
      name: Fan Speed Error

    allow_battery_charging:
      name: Allow Battery Charging
    solar_battery_charging:
      name: Solar Battery Charging
    grid_battery_charging:
      name: Grid Battery Charging

    fan1_on:
      name: Fan 1 On
    fan2_on:
      name: Fan 2 On

text_sensor:
  - platform: u_inverter
    operating_mode:
      name: Operating Mode
      filters:
        - map:
            - P -> Power On
            - S -> Standby
            - L -> On Grid
            - B -> On Battery
            - F -> System Fault
            - D -> Shutdown
            - X -> Test
```
