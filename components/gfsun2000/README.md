# GFSun2000 inverter modbus interface

A configured uart component is required.
A configured modbus component is usually optional.  It will be automatically created.

Example:
```yaml
sensor:
  - platform: gfsun2000
    id: my_gfsun
    device_id:
      id: did
    output_power:
      id: op
```

Optional text sensor:
- `device_id`

All sensors are optional.  The available sensors are:
- `ac_voltage`
- `ac_voltage`
- `dc_voltage`
- `output_power`
- `custom_energy`
- `total_energy`

