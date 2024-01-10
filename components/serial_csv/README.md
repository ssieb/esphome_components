# A component that reads CSV values from the uart.

A configured uart is required.

Configure a list of sensors.  The index is required, the rest is the standard sensor config.

Example:
```yaml
sensor:
  - platform: serial_csv
    uart_id: my_uart   # optional
    sensors:
      - index: 0
        name: First value
      - index: 3
        name: Fourth value
```

