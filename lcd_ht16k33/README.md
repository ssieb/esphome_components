# HT16K33 4 character alphanumeric display

There are no print functions for addressing rows and columns.  With such a small display I didn't see much point.
All the print functions without the row and column parameters are available.
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
    lambda: |-
      it.print("ABCD");
```

