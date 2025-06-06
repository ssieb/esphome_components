# text_sensor that reads lines for a uart. Also, a sensor that reads single values from the uart.

A configured uart is required.

Example:
```yaml
text_sensor:
  - platform: serial
    uart_id: my_uart   # optional
    name: My Serial data

sensor:
  - platform: serial
    uart_id: my_uart   # optional
    name: My Serial number
```

