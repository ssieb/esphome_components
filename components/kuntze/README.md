# Kuntze pool monitor modbus interface

A configured uart component is required.
A configured modbus component is usually optional.  It will be automatically created.

Example:
```yaml
sensor:
  - platform: kuntze
    id: my_kuntze
    ph:
      id: ph
    temperature:
      id: temperature
```

All sensors are optional.  The available sensors are:
- `ph`
- `temperature`
- `dis1`
- `dis2`
- `redox`
- `ec`
- `oci`

