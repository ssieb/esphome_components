# A component for a certain pedestal fan

This component requires a configured `remote_transmitter`.

Example:
```yaml
remote_transmitter:
  id: remote
  pin: 32
  carrier_duty_percent: 100

fan:
  - platform: pedestal
    name: Pedestal Fan
    speed_pin: 17
    osc_pin: 16
    transmitter_id: remote
```
