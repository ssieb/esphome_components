# input_builder component

This component collects key presses from a `key_provider` like the `matrix_keypad` or `wiegand` components.

You need at least one of the `end_keys` or `max_length` parameters.  The rest are optional.

Example:
```yaml
input_builder:
  - id: pin_reader
    source_id: mykeypad
    min_length: 4
    max_length: 4
    start_keys: ""  # When set the builder start only when the given key is pressed first.
    end_keys: "#"
    end_key_required: true   # default is false
    back_keys: "*"
    clear_keys: "C"
    timeout: 5s
    allowed_keys: "0123456789"  # if not included, then any otherwise unused keys will be allowed
    on_progress:
      - logger.log: 
          format: "input progress: '%s'"
          args: [ 'x.c_str()' ]
    on_result:
      - logger.log: 
          format: "input result: '%s'"
          args: [ 'x.c_str()' ]
```

