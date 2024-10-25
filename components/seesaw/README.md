# Seesaw component

This component provides support for the Adafruit Seesaw platform.  Define a `seesaw` component then add components for each function you want to use.

Currently, the supported functions are the rotary encoder, rotary encoder button, temperature, touch, and single neopixel LED.

Example:
```yaml
seesaw:

sensor:
  - platform: seesaw
    id: encoder
    type: encoder
    name: "Seesaw encoder"
    number: 1  # optional, defaults to 0
  - platform: seesaw
    id: temp
    type: temperature
  - platform: seesaw
    id: touch
    type: touch
    pin: 5

binary_sensor:
  - platform: seesaw
    id: button
    name: "Seesaw encoder button"
    pin: 2

light:
  - platform: seesaw
    id: pixel
    name: "Seesaw neopixel LED"
    pin: 14
```

