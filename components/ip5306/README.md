# IP5306 battery charger

This component provides support for the IP5306 battery charger IC.

You need an `i2c:` component configured.  All the sensors are optional.

Example:
```yaml
ip5306:
  battery_level:   # sensor
    name: Battery Level
  charger_connected:  # binary_sensor
    id: connected
    on_press:
      then:
        - lambda: ESP_LOGD("TEST", "charging");
    on_release:
      then:
        - lambda: ESP_LOGD("TEST", "not charging");
  charge_full:  # binary_sensor
    id: full
    on_press:
      then:
        - lambda: ESP_LOGD("TEST", "fully charged");
    on_release:
      then:
        - lambda: ESP_LOGD("TEST", "still charging");
```
