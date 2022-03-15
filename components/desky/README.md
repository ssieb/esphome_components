# Desky sit/stand desk component

This component provides support for reading the Desky sit/stand desk height

Example:
```yaml
uart:
  - id: desk_uart
    baud_rate: 9600
    rx_pin: 3

sensor:
  - platform: desky
    #uart_id: desk_uart  (optional, unless multiple uarts are defined)
    name: Desk Height
    # any other sensor options
```
