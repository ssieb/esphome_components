# SM2235 LED driver

Example:
```yaml
sm2235:
  - id: my_sm        #optional
    sda: 4
    scl: 5
    rgb_current: 12  #optional
    cw_current: 25   #optional

output:
  - platform: sm2235
    id: red
    channel: 0
  - platform: sm2235
    id: green
    channel: 1
  - platform: sm2235
    id: blue
    channel: 2

light:
  - platform: rgb
    name: My Light
    red: red
    green: green
    blue: blue
```

