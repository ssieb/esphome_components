# component to read and control an ETA SH system

A configured uart is required.

Example:
```yaml
eta_sh:
  boiler_temperature:
    name: Boiler Temperature
  return_temperature:
    name: Return Temperature
  buffer_below_temperature:
    name: Buffer Below Temperature
  buffer_middle_temperature:
    name: Buffer Middle Temperature
  buffer_above_temperature:
    name: Buffer Above Temperature
```

All sensors are optional.

