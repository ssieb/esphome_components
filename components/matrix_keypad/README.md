# keypad component

This component is for matrix keypads.  Define a `keypad` component then add `binary_sensor`s to handle individual keys.  You need to also import the `key_provider` component.
If you want automatic handling for multiple keys, e.g. PIN entry, use the `key_collector` component.

The `keys` parameter is optional for the `keypad`, but then you won't be able to check for it in the `binary_sensor`
and the `input_builder` won't work if you want to use that.
The optional `has_diodes` parameter is for if the buttons have diodes and the row pins are output only. In that case, set it to true.

For the `binary_sensor`, you need to provide either the `row` and `col` parameters or the `key` parameter.

Example:
```yaml
matrix_keypad:
  id: mykeypad
  rows:
    - pin: 21
    - pin: 19
    - pin: 18
    - pin: 5
  columns:
    - pin: 17
    - pin: 16
    - pin: 4
    - pin: 15
  keys: "123A456B789C*0#D"
  has_diodes: false

binary_sensor:
  - platform: keypad
    keypad_id: mykeypad
    id: key4
    row: 1
    col: 0
  - platform: keypad
    id: keyA
    key: A
```

The component also supports the use of a PIN schema which allows for connectivity via sx1509 expander, should also work for other expander platforms as long as the PIN schema is handled.

Example sx1509 
```yaml
matrix_keypad:
  id: garagekeypad
  rows: 
    - pin:
        sx1509: sx1509_hub1
        number: 0
    - pin:
        sx1509: sx1509_hub1
        number: 1
    - pin:
        sx1509: sx1509_hub1
        number: 2
    - pin:
        sx1509: sx1509_hub1
        number: 3
  columns:
    - pin:
        sx1509: sx1509_hub1
        number: 8
    - pin:
        sx1509: sx1509_hub1
        number: 9
    - pin:
        sx1509: sx1509_hub1
        number: 10
    - pin:
        sx1509: sx1509_hub1
        number: 11
  keys: "123A456B789C*0#D"
```