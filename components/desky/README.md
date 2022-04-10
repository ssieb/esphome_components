# Desky sit/stand desk component

This component provides support for reading the Desky sit/stand desk height and moving it to specific heights

Example:
```yaml
uart:
  - id: desk_uart
    baud_rate: 9600
    rx_pin: 3

desky:
  #uart_id: desk_uart  (optional, unless multiple uarts are defined)
  id: my_desky
  height:  # optional sensor publishing the current height
    name: Desk Height
    # any other sensor options
  up: 2    # optional <pin> config
  down: 4  # optional <pin> config
  stopping_distance: 15  # optional distance from target to turn off moving, default 15

on_...:
  then:
    - lambda: id(my_desky).move_to(150);
```
