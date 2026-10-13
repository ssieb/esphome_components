# HT16K33 4 character 7-segment display

This component supports the 4 character (plus colon) 7 segment character display.

> :warning: If using this component as an external component, you need to include both `ht16k33_7segment` **and** `ht16k33_base` components.
```yaml
external_components:
  source: github://ssieb/custom_components/
  components: [ ht16k33_base, ht16k33_7segment ]
```

There are no print functions for addressing rows and columns.  With such a small display I didn't see any point.
All the print functions without the row and column parameters are available.
A "." will get added to the previous character as the decimal point.
All the same parameters for the i2c display can be used other than the dimensions.
There are also lambda functions `get_brightness` and `set_brightness` for adjusting the brightness of the display.
You can extend the display across multiple units.

The colon in the middle of the display will be lit if the print string contains a ":" at the 3rd position (e.g. 12:34).

Example:
```yaml
i2c:
  sda: D0
  scl: D1

display:
  - platform: ht16k33_7segment
    address: 0x70
    scroll: true
    scroll_speed: 250ms
    scroll_dwell: 2s
    scroll_delay: 3
    lambda: |-
      auto time = id(time_sensor).now();
      it.strftime("%H:%M", time);
```

# Optional parameters

`scroll:` defaults to false

`scroll_speed:` is the time between each movement, default 250ms

`scroll_dwell:` is the time to wait at the end before going back to the start, default 2s

`scroll_delay:` is the number (float, minimum 1) of `scroll_speed` cycles to wait at the beginning before starting to scroll, default 3

`secondary_display:` is a list of i2c devices where `address:` is required and `i2c_id:` is optional unless there is more than one i2c bus.

