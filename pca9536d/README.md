# PCA9536D i2c I/O expander

Requires a configured i2c bus

Example:
```yaml
pca9536d:
  - id: my_pca

switch:
  - platform: gpio
    pin:
      pca9536d: my_pca
      number: 0
    id: relay

binary_sensor:
  - platform: gpio
    pin:
      pca9536d: my_pca
      number: 1
    id: button
```

# Optional parameters

`address:` defaults to 0x41

