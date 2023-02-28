# This component has been merged to esphome: <https://next.esphome.io/components/key_collector.html>

# key_collector component

This component collects key presses from a `key_provider` like the `matrix_keypad` or `wiegand` components.

You need at least one of the `end_keys` or `max_length` parameters.  The rest are optional.
There is a `clear()` method with an optional `bool` (default true) parameter for whether or not to trigger the progress action.

Example:
```yaml
key_collector:
  - id: pin_reader
    source_id: mykeypad
    min_length: 4
    max_length: 4
    start_keys: "A"
    end_keys: "#"
    end_key_required: true   # default is false
    back_keys: "*"
    clear_keys: "C"
    timeout: 5s
    allowed_keys: "0123456789"  # if not included, then any otherwise unused keys will be allowed
    on_progress:
      - logger.log: 
          format: "input progress: '%s', started by '%c'"
          args: [ 'x.c_str()', 'start' ]
    on_result:
      - logger.log: 
          format: "input result: '%s', started by '%c', ended by '%c'"
          args: [ 'x.c_str()', 'start', 'end' ]
```

