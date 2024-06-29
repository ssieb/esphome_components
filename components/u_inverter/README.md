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
```
