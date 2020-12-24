# Growatt inverter modbus interface

A configured uart component is required.
A configured modbus component is usually optional.  It will be automatically created.

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
- `input_power`
- `pv1_voltage`
- `pv1_current`
- `pv1_power`
- `pv2_voltage`
- `pv2_current`
- `pv2_power`
- `output_power`
- `grid_frequency`
- `ac_voltage`
- `ac_current`
- `ac_power`
- `today_gen`
- `total_gen`
- `temperature`
- `today_grid`
- `total_grid`

