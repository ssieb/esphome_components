# A component that reads CSV values from the uart.

A configured uart is required.

Configure a list of sensors and text sensors.  The index is required, the rest is the standard sensor or text sensor config.

Example:
```yaml
serial_csv:
  uart_id: my_uart   # optional
  sensors:
    - index: 0
      name: First value
    - index: 3
      name: Fourth value
  text_sensors:
    - index: 1
      name: Second value
    - index: 2
      name: Third value
```

