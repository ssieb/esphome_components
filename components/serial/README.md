# text_sensor that reads lines for a uart

A configured uart is required.

Example:
```yaml
text_sensor:
  - platform: serial
    uart_id: my_uart   # optional
    name: My Serial data
```

