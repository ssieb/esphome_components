# Seesaw component

This component provides support for the Adafruit Seesaw platform.  Define a `seesaw` component then add components for each function you want to use.

Currently, the supported functions are the rotary encoder, the rotary encoder button, and the single neopixel LED.

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

light:
  - platform: seesaw
    id: pixel
    name: "Seesaw neopixel LED"
```

