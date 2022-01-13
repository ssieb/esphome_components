# Wemos OLED shield buttons

This component provides support for the buttons on the [Wemos OLED shield](https://www.wemos.cc/en/latest/d1_mini_shield/oled_0_66.html).

You need an `i2c:` component configured.  All the triggers are optional.

Example:
```yaml
i2c_button:
  a:
    on_click:
      - lambda: ESP_LOGD("BUTTON", "Button A clicked");
    on_long_click:
      - lambda: ESP_LOGD("BUTTON", "Button A long clicked");
    on_double_click:
      - lambda: ESP_LOGD("BUTTON", "Button A double clicked");
  b:
    on_click:
      - lambda: ESP_LOGD("BUTTON", "Button B clicked");
    on_long_click:
      - lambda: ESP_LOGD("BUTTON", "Button B long clicked");
    on_double_click:
      - lambda: ESP_LOGD("BUTTON", "Button B double clicked");
```
