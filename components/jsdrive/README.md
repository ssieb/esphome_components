# component that reads data from a jsdrive controlled desk

Two configured uarts are required for full function.

Example:
```yaml
jsdrive:
  id: my_jsdrive
  desk_uart: desk_bus
  remote_uart: remote_bus
  message_length: 6    # optional, 5 or 6, default is 6
  height:
    name: Desk Height
  up:
    name: Up Button
  down: 
    name: Down Button
  memory1:
    name: Memory1 Button
  memory2:
    name: Memory2 Button
  memory3:
    name: Memory3 Button
```

`height` is a `sensor`, the others are `binary_sensor`s indicating the button states.

There are methods `move_to(height)` and `stop()` that you can use in a lambda.

