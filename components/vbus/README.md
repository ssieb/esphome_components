# VBus protocol component

A configured uart component is required.

Example:
```yaml
vbus:
  uart_id: the_uart

sensor:
  - platform: vbus
```

The `uart_id` is optional.

All sensors are optional.
```
