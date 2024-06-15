# AXP2101

This component provides basic support for the AXP2101 chip.  Only battery level and charging status.

You need an `i2c:` component configured.

Example:
```yaml
sensor:
  - platform: axp2101
    battery_level:
      name: Battery Level
    charging:
      name: Charging Status
```
