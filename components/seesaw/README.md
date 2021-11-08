# Seesaw component

This component provides support for the Adafruit Seesaw platform.  Define a `seesaw` component then add components for each function you want to use.

Currently, the only supported function is the rotary encoder.

Example:
```yaml
seesaw:

sensor:
  - platform: seesaw
    id: encoder
    name: "Seesaw encoder"

```

