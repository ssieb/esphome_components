# demo component to show how to implement a hub component with various sub-components

A configured uart is required.

Example:
```yaml
uart_demo:
  - id: demo
    uart_id: demo_uart

text_sensor:
  - platform: uart_demo
    id: the_text
    name: UART demo text sensor

sensor:
  - platform: uart_demo
    id: the_sensor
    name: UART demo sensor

output:
  - platform: uart_demo
    id: the_output
    name: UART demo output

binary_sensor:
  - platform: uart_demo
    id: the_binsensor
    name: UART demo binary sensor

switch:
  - platform: uart_demo
    id: the_switch
    name: UART demo switch

button:
  - platform: uart_demo
    id: the_button
    name: UART demo button

light:
  - platform: monochromatic
    name: Float Light
    output: the_output

interval:
  - interval: 10s
    then:
      - switch.toggle: the_switch
      - button.press: the_button
```

