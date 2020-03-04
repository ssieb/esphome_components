# shift register component

This component should work for most shift register chips.  Define a `shift_register` component with the needed pins, then wherever a pin
is needed, you can use one of the outputs.

Example:
```yaml
shift_register:
  id: shift1
  data_pin: D0
  clock_pin: D1

switch:
  - platform: gpio
    name: "Pin 1"
    pin:
      shift_register: shift1
      number: 1
  - platform: gpio
    name: "Pin 2"
    pin:
      shift_register: shift1
      number: 2

output:
  - platform: gpio
    pin:
      shift_register: shift1
      number: 7
    id: output_component1

light:
  - platform: binary
    name: "Desk Lamp"
    output: output_component1

```

