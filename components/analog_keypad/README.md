# A component for analog keypads

This component is for analog keypads that represent key presses by varying voltage levels.
Define an `analog_keypad` component then add `binary_sensor`s to handle individual keys.
If you want automatic handling for multiple keys, e.g. PIN entry, use the `key_collector` component and provide key values for the buttons.

For the `binary_sensor`, you need to provide either the `index` parameter or the `key` parameter.

Example:
```yaml
sensor:
  - platform: adc
    id: keypad_adc
    pin: A0
    update_interval: never

analog_keypad:
  sensor: keypad_adc
  debounce_time: 100
  keys:
    - voltage: 1.5    # center voltage
      variance: 0.1   # amount it can vary from the center voltage
      key: A
    - voltage: 2
      variance: 0.1
  
binary_sensor:
  - platform: analog_keypad
    id: second_key
    index: 1
  - platform: analog_keypad
    id: first_key
    key: A
```
