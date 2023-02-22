# SDI12 sensor component

A configured uart component is required.

Example:
```yaml
external_components:
  - source: github://ssieb/esphome@uarthalf
    components: [ uart ]
    refresh: 1min
  - source:
      type: git
      url: https://github.com/ssieb/custom_components@sdi12
    components: [ sdi12 ]
    refresh: 1min

uart:
  - id: sdi12uart
    tx_pin: 13
    rx_pin: 12
    baud_rate: 1200
    data_bits: 7
    parity: even
    half_duplex: true

sdi12:
  - uart_id: sdi12uart

sensor:
  - platform: sdi12
    address: 5
    update_interval: 10s
    sensors:
      index: 1
      name: blah
```

The `uart_id` is optional.

The sensors are indexed from 1-9.

For the uart, the `tx_pin` is the active one, the `rx_pin` needs to be some other unused pin.

