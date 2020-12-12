# Growatt inverter modbus interface

A configured uart component is required.
A configured modbus component is usually optional.

Example:
```yaml
sensor:
  - platform: growatt
    id: my_growatt
    input_power:
      id: inv
    pv1_voltage:
      id: pv1_volt
```

All sensors are optional.  The available sensors are:
- `input_power_high`
- `input_power_low`
- `pv1_voltage`
- `pv1_current`
- `pv1_power_high`
- `pv1_power_low`
- `pv2_voltage`
- `pv2_current`
- `pv2_power_high`
- `pv2_power_low`
- `grid_frequency`
- `output_power_high`
- `output_power_low`
- `temperature`

