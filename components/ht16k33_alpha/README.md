# HT16K33 4 character alphanumeric display

There are no print functions for addressing rows and columns.  With such a small display I didn't see any point.
All the print functions without the row and column parameters are available.
A "." will get added to the previous character as the decimal point.
All the same parameters for the i2c display can be used other than the dimensions.
There are also lambda functions `get_brightness` and `set_brightness` for adjusting the brightness of the display.
You can extend the display across multiple units.

Example:
```yaml
i2c:    
  sda: D0
  scl: D1

display:
  - platform: ht16k33_alpha
    address: 0x70
    scroll: true
    scroll_speed: 250ms
    scroll_dwell: 2s
    scroll_delay: 3
    c_scroll: false
    c_scroll_display_length: 4
    c_scroll_spacer: "-."
    lambda: |-
      it.print("ABCD");
    secondary_displays:
      - address: 0x71
    custom_buffer: 64
```

# Optional parameters

`scroll:` defaults to false

`scroll_speed:` is the time between each movement, default 250ms

`scroll_dwell:` is the time to wait at the end before going back to the start, default 2s

`scroll_delay:` is the number (float, minimum 1) of `scroll_speed` cycles to wait at the beginning before starting to scroll, default 3

`c_scroll:` "continuous scrolling", default to false

`c_scroll_display_length:` is the amount of characters the display is able to show at once, default 4 (only needed if `c_scroll:` is enabled)

`c_scroll_spacer:` is the text displayed between the end and the beginning of the continuous scroll, default "-." (only needed if `c_scroll:` is enabled)

`secondary_display:` is a list of i2c devices where `address:` is required and `i2c_id:` is optional unless there is more than one i2c bus.

`custom_buffer:` is a custom buffer size (how large the "text to display" is able to be), default 64
