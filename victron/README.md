# Victron solar charge controller

A configured uart component is required.

Example:
```yaml
victron:
  uart_id: the_uart

sensor:
  - platform: victron
    panel_voltage:
      id: pv
    battery_voltage:
      id: bv
    battery_current:
      id: bc
```

The `uart_id` is optional.

All sensors are optional.

The available numeric sensors are:
- `max_power_yesterday`
- `max_power_today`
- `yield_total`
- `yield_yesterday`
- `yield_today`
- `panel_voltage`
- `panel_power`
- `battery_voltage`
- `battery_current`
- `day_number`
- `charger_status`
- `error_code`

The available text sensors are:
- `charger_text`
- `error_text`
- `tracker_operation`
- `fw_version`
- `pid`
```
