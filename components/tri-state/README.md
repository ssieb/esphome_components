# GPIO switch that changes to input mode when off

Example:
```yaml
switch:
  - platform: tri-state
    name: My Switch
    pin: 12
    active_state: true   # required.  This defines the output state when the switch is on. Set as true for high (VCC) or false for low (GND)
```

