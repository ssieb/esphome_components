# ADC multiplexor

A component to multiplex a single adc pin

Example:
```yaml
sensor:
  - platform: adc
    id: source
    pin: A0
  - platform: adc_mpx
    id: s1
    sensor: source
    pin: D1
  - platform: adc_mpx
    id: s2
    sensor: source
    pin: D2
```

