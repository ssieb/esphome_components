# IFan04 component

This component provides support for the IFan04 remote control

Example:
```yaml
uart:
  - id: fan_remote
    baud_rate: 9600
    rx_pin: 3

ifan04:
  on_fan:
    - lambda: ESP_LOGD("IFAN", "speed is %d", speed);
  on_light:
    - lambda: ESP_LOGD("IFAN", "light trigger");
  on_buzzer:
    - lambda: ESP_LOGD("IFAN", "buzzer trigger");
```
