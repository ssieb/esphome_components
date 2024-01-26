# CardKB: https://shop.m5stack.com/products/cardkb-mini-keyboard

This component is for the CardKB device.  Define a `cardkb` component then add `binary_sensor`s to handle individual keys.
If you want automatic handling for multiple keys, e.g. PIN entry, use the `key_collector` component.

Example:
```yaml
cardkb:
  - id: mykb  #optional

binary_sensor:
  - platform: cardkb
    cardkb_id: mykb  #optional if only one keyboard
    id: a_key
    key: a
  - platform: cardkb
    id: enter_key
    key: 13
```

