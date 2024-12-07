# Desky sit/stand desk component

This component provides support for reading the Desky sit/stand desk height and moving it to specific heights

Example:
```yaml
uart:
  - id: desk_uart
    baud_rate: 9600
    rx_pin: 14
    tx_pin: 13  # if you want to passthrough to the display

desky:
  #uart_id: desk_uart  (optional, unless multiple uarts are defined)
  id: my_desky
  height:  # optional sensor publishing the current height
    name: Desk Height
    # any other sensor options
  up_pin: 4  # optional <pin> config
  down_pin: 5 # optional <pin> config
  request_pin: 12  # optional <pin> config to request height updates at boot
  up_in_pin: 16  # optional <pin> config to read the up button from the display
  down_in_pin: 17  # optional <pin> config to read the down button from the display
  stopping_distance: 15  # optional distance from target to turn off moving, default 15
  timeout: 15s  # optional time limit for moving, default is none
  intercept: true  # option, default to false.  If false, will pass the display button states to the desk if not currently moving

on_...:
  then:
    - lambda: id(my_desky).move_to(150);
    - lambda: id(my_desky).stop();

binary_sensor:
  - platform: template
    name: Desky moving
    lambda: return id(my_desky).current_operation != desky::DESKY_OPERATION_IDLE;
```
