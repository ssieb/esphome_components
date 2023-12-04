# M5 8encoder component

This component provides support for the M5Stack 8encoder device.  Define a `m5_8encoder` component then add components for each function you want to use.

Example:
```yaml
m5_8encoder:

sensor:
  - platform: m5_8encoder
    id: encoder0
    number: 0
    name: "first encoder"
    min_value: 0
    max_value: 255
  - platform: m5_8encoder
    id: encoder7
    number: 7
    name: "last encoder"

binary_sensor:
  - platform: m5_8encoder
    id: toggle
    name: "toggle button"

light:
  - platform: m5_8encoder
    id: led0
    number: 0
    name: "first LED"
  - platform: m5_8encoder
    id: led8
    number: 8
    name: "toggle switch LED"
```

