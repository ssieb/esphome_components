# A component for a certain pedestal fan

This component requires a configured `duty_cycle` sensor and a `remote_transmitter`.

Example:
```yaml
remote_transmitter:
  id: remote
  pin: 32
  carrier_duty_percent: 100

sensor:
  - platform: duty_cycle
    id: duty
    pin: 17

fan:
  - platform: pedestal
    name: Pedestal Fan
    pulse: duty
    osc_pin: 16
    transmitter_id: remote
```
