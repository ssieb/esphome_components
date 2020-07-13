# EZO i2c sensor circuit reader

An i2c component is required to be configured.

Example:
```yaml
sensor:
  - platform: ezo
    id: ph_ezo
    address: 99
    unit_of_measurement: "pH"
```

You can set the temperature compensation value using `id(ph_ezo).set_tempcomp_value(temperature);` in a lambda.

