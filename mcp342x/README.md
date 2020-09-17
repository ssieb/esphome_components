# MCP342X sensor component

An i2c component is required to be configured.

Example:
```yaml
mcp342x:

sensor:
  - platform: mcp342x
    id: adc1
    channel: 1
    gain: 2
    nbits: 14
```

