# TOTP component

A configured time component is required and this only works on ESP32.

Example:
```yaml
totp:
  - secret: the_secret
    totp:
      name: TOTP
    countdown:
      name: Countdown
```

