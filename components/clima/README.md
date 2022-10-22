# component to handle the data from a Clima Sensor D

A configured uart is required.

Example:
```yaml
clima:
  time:                  # text sensor
    name: Current Time
  brightness_east:
    name: Brightness East
  brightness_west:
    name: Brightness West
  brightness_south:
    name: Brightness South
  twilight:
    name: Twilight Brightness
  humidity:
    name: Humidity
  temperature:
    name: Temperature
  precipitation:         # binary sensor
    name: Is Precipitation
  wind_speed:
    name: Wind Speed
  low_voltage:           # binary sensor
    name: Is Low Voltage
  no_time_sync:          # binary sensor
    name: Time Sync Failed
  heating:               # binary sensor
    name: Is Heating
  time_syncing:          # binary sensor
    name: Syncing Time
```

All sensors are optional.

