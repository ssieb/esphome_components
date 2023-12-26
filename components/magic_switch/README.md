# magic switch

A component attempting to support the magic switch option of the Sonoff Basic R4. GPIO5 seems to be the correct pin.

Example:
```yaml
magic_switch:
  pin: 5
  timeout: 13000us  #optional, default is 12000us
  on_switch:
    # do something
```

