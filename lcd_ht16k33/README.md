# HT16K33 4 character alphanumeric display

There are no print functions for addressing rows and columns.  With such a small display I didn't see any point.
All the print functions without the row and column parameters are available.
A "." will get added to the previous character as the decimal point.
All the same paramters for the i2c lcd display can be used other than the dimensions.
There are also lambda functions `get_brightness` and `set_brightness` for adjusting the brightness of the display.

Example:
```yaml
i2c:    
  sda: D0
  scl: D1

display:
  - platform: lcd_ht16k33
    address: 0x70
    scroll: true
    scroll_speed: 250ms
    scroll_dwell: 2s
    lambda: |-
      it.print("ABCD");
```

`scroll:` defaults to false

`scroll_speed:` is the time between each movement, default 250ms

`scroll_dwell:` is the time to wait at the end before going back to the start, default 2s

