# APC Smart UPS serial interface
# This is completely untested!

A configured uart component is required.

Example:
```yaml
sensor:
  - platform: apcups
    id: myups
    battery_voltage:
      id: bat
    input_voltage:
      id: inv
```

All sensors are optional.  The available sensors are:
- `internal_temp`
- `battery_voltage`
- `battery_level`
- `power_load`
- `input_voltage`
- `output_voltage`
- `line_frequency`
- `estimated_runtime`

