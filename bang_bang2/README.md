# alternate bang_bang climate component

This climate controller works more like a normal thermostat.
The high temperature controls the cooling and the low temperature controls the heating.
There is a hysteresis value to set the range that the temperature can vary.
the config is the same as the included bang_bang component, except for the optional hysteresis which defaults to 0.5.

Example:
```yaml
climate:
  - platform: bang_bang2
    id: bang
    sensor: temp_sensor
    default_target_temperature_low: 20 °C
    default_target_temperature_high: 24 °C
    hysteresis: 0.5 °C

    heat_action:
      - switch.turn_on: heater
    idle_action:
      - switch.turn_off: heater
```

