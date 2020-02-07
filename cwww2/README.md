# alternate cwww component

This is for lights that don't have separate pwm outputs for each color,
but instead have brightness and color temperature pwm outputs.

Example:
```yaml
light:
  - platform: cwww2
    id: mylight
    name: "My Light"
    brightness: output1
    color_temperature: output2
    cold_white_color_temperature: 6536 K
    warm_white_color_temperature: 2000 K
```

The gamma correct is defaulted to 1.0 as these lights appear to handle that internally.

