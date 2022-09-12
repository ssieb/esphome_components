# Wiegard card reader

Reads a card or key presses from a Wiegand interface.
You must also import the `key_provider` component.

If you want automatic handling for multiple keys, e.g. PIN entry, use the `input_builder` component.

Example:
```yaml
wiegand:
  - id: reader
    d0: 4
    d1: 5
    on_tag:
      - lambda: ESP_LOGD("TEST", "received tag %s", x.c_str());
    on_key:
      - lambda: ESP_LOGD("TEST", "received key %d", x);
```

