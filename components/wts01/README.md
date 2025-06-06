# Sonoff WTS01 (Dallas temp behind an MCU)

Requires a configured uart with an RX pin at 9600 baud.

Example:
```yaml
sensor:
  - platform: wts01
    uart_id: the_uart  # optional
    name: The Temperature
```

