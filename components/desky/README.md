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
  up:    # optional <pin> config
    number: 4
    inverted: true  # probably needed
  down:  # optional <pin> config
    number: 5
    inverted: true  # probably needed
  request:  # optional <pin> config to request height updates at boot
    number: 12
    inverted: true  # probably needed
  stopping_distance: 15  # optional distance from target to turn off moving, default 15
  timeout: 15s  # optional time limit for moving, default is none

on_...:
  then:
    - lambda: id(my_desky).move_to(150);
    - lambda: id(my_desky).stop();

binary_sensor:
  - platform: template
    name: Desky moving
    lambda: return id(my_desky).current_operation != desky::DESKY_OPERATION_IDLE;
```
