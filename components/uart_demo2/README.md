# demo component to show how to include various other components in a top-level component.  This method is not recommended.

A configured uart is required.

Example:
```yaml
uart_demo2:
  - id: demo
    uart_id: demo_uart
    the_text:
      id: the_text
    the_sensor:
      id: the_sensor
    the_bin_output:
      id: the_bin_out
    the_flt_output:
      id: the_flt_out
    the_binsensor:
      id: the_binsensor
    the_switch:
      id: the_switch
    the_button:
      id: the_button

light:
  - platform: binary
    name: Binary Light
    output: the_bin_out
  - platform: monochromatic
    name: Float Light
    output: the_flt_out

interval:
  - interval: 10s
    then:
      - switch.toggle: the_switch
      - button.press: the_button
```

