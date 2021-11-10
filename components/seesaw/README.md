# Seesaw component

This component provides support for the Adafruit Seesaw platform.  Define a `seesaw` component then add components for each function you want to use.

Currently, the only supported functions are the rotary encoder and the rotary encoder button.

Example:
```yaml
seesaw:

sensor:
  - platform: seesaw
    id: encoder
    name: "Seesaw encoder"

binary_sensor:
  - platform: seesaw
    id: button
    name: "Seesaw encoder button"

```

