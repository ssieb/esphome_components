# JDB BMS component

A configured uart component is required.

Example:
```yaml
sensor:
  - platform: jdb_bms
    voltage:
      name: Voltage
    current:
      name: Current
    capacity:
      name: Capacity
    battery_voltage:
      - string: 1   # 1-based
        name: String 1 voltage
    temperature:
      - number: 0   # 0-based
        name: NTC 0 temp
    balance_capacity:
      name: Balance Capacity
    rate_capacity:
      name: Rate Capacity
```

