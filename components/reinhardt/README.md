# A component for reading data from Reinhardt weather stations

A configured uart is required.

Example:
```yaml
sensor:
  - platform: reinhardt
    uart_id: my_uart   # optional
    temperature:
      name: Temperature
    humidity:
      name: Humidity
    indoor_temperature:
      name: Indoor Temperature
    pressure:
      name: Pressure
    wind_direction:
      name: Wind Direction
    prevailing_wind_direction:
      name: Prevailing Wind Direction
    wind_speed:
      name: Wind Speed
    peak_wind_speed:
      name: Peak Wind Speed
    average_wind_speed:
      name: Average Wind Speed
    rainfall:
      name: Rainfall Amount
    rainfall_over_interval:
      name: Rainfall Over An Interval
    solar_radiation:
      name: Solar Radiation
    uv_radiation:
      name: UV Radiation
    light:
      name: Light Level (Lux)
    barometer:
      name: Barometer
    dewpoint:
      name: Dewpoint
    windchill:
      name: Wind Chill
    altitude:
      name: Altitude
    gps_altitude:
      name: Altitude from GPS
    latitude:
      name: Latitude
    longitude:
      name: Longitude
    speed:
      name: Speed
    satellites:
      name: Number of Satellites
```

