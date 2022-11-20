# component to read and control an ETA SH system

A configured uart is required.

Example:
```yaml
eta_sh:
  boiler_temperature:
    name: Boiler Temperature
  return_temperature:
    name: Return Temperature
  buffer_bottom_temperature:
    name: Buffer Bottom Temperature
  buffer_middle_temperature:
    name: Buffer Middle Temperature
  buffer_top_temperature:
    name: Buffer Top Temperature
  exhaust_temperature:
    name: Exhaust Temperature
  outside_temperature:
    name: Outside Temperature
  buffer_load:
    name: Buffer Load
```

All sensors are optional.

