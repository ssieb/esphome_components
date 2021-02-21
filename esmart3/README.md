# ESmart3 charge controller

A configured uart component is required.

Example:
```yaml
sensor:
  - platform: esmart3
    charge_mode:
      id: cm
    input_voltage:
      id: iv
    battery_voltage:
      id: bv
    charging_current:
      id: cc
    load_voltage:
      id: lv
    load_current:
      id: lc
    charging_power:
      id: cp
    load_power:
      id: lp
    battery_temp:
      id: bt
    internal_temp:
      id: it
    battery_level:
      id: bl
```

All sensors are optional.  The available sensors are:
- `charge_mode`
- `input_voltage`
- `battery_voltage`
- `charging_current`
- `load_voltage`
- `load_current`
- `charging_power`
- `load_power`
- `battery_temp`
- `internal_temp`
- `battery_level`
```

