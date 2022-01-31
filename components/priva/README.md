# Priva temperature and humidity sensor

A configured uart component is required.

Note:
  This is very simple parsing and no checksum verification

Example:
```yaml
sensor:
  - platform: priva
    temperature:
      id: temp
      name: Temperature
    humidity:
      id: hum
      name: Humidity
```

