# GPIO switch that changes to input mode when off

Example:
```yaml
switch:
  - platform: tri-state
    name: My Switch
    pin: 12
    on_state: true   # required.  This defines the output state when the switch is on.
```

