# Wiegard card reader

Reads a card number from a Wiegand interface

Example:
```yaml
wiegand:
  - id: reader
    d0: 4
    d1: 5
    on_tag:
      - lambda: ESP_LOGD("TEST", "received tag %s", x.c_str());
```

