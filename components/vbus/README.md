# VBus protocol component

A configured uart component is required.

Example:
```yaml
vbus:
  uart_id: the_uart

sensor:
  - platform: vbus
    model: deltasol c
    temperature_1:
      name: Temperature 1
    temperature_2:
      name: Temperature 2
    temperature_3:
      name: Temperature 3
    temperature_4:
      name: Temperature 4
    pump_speed_1:
      name: Pump Speed 1
    pump_speed_2:
      name: Pump Speed 2
    operating_hours_1:
      name: Operating Hours 1
    operating_hours_2:
      name: Operating Hours 2
    heat_quantity:
      name: Heat Quantity
    time:
      name: System Time

  - platform: vbus
    model: deltasol cs2
    temperature_1:
      name: Temperature 1
    temperature_2:
      name: Temperature 2
    temperature_3:
      name: Temperature 3
    temperature_4:
      name: Temperature 4
    pump_speed:
      name: Pump Speed
    operating_hours:
      name: Operating Hours
    heat_quantity:
      name: Heat Quantity
    version:
      name: SW Version

  - platform: vbus
    model: deltasol_bs_plus
    temperature_1:
      name: Temperature 1
    temperature_2:
      name: Temperature 2
    temperature_3:
      name: Temperature 3
    temperature_4:
      name: Temperature 4
    pump_speed_1:
      name: Pump Speed 1
    pump_speed_2:
      name: Pump Speed 2
    operating_hours_1:
      name: Operating Hours 1
    operating_hours_2:
      name: Operating Hours 2
    heat_quantity:
      name: Heat Quantity
    time:
      name: System Time
    version:
      name: SW Version

  - platform: vbus
    model: custom
    command: 0x100
    source: 0x1234
    dest: 0x10
    lambda: |-
      // the data is in `x`

text_sensor:
  - platform: vbus
    model: deltasol c
    time:
      name: System Time

  - platform: vbus
    model: deltasol cs2
    version:
      name: Version

  - platform: vbus
    model: deltasol_bs_plus
    time:
      name: System Time
    version:
      name: Version

binary_sensor:
  - platform: vbus
    model: deltasol c
    sensor1_error:
      name: Sensor 1 Error
    sensor2_error:
      name: Sensor 2 Error
    sensor3_error:
      name: Sensor 3 Error
    sensor4_error:
      name: Sensor 4 Error

  - platform: vbus
    model: deltasol cs2
    sensor1_error:
      name: Sensor 1 Error
    sensor2_error:
      name: Sensor 2 Error
    sensor3_error:
      name: Sensor 3 Error
    sensor4_error:
      name: Sensor 4 Error

  - platform: vbus
    model: deltasol_bs_plus
    relay1:
      name: Relay 1 On
    relay2:
      name: Relay 2 On
    sensor1_error:
      name: Sensor 1 Error
    sensor2_error:
      name: Sensor 2 Error
    sensor3_error:
      name: Sensor 3 Error
    sensor4_error:
      name: Sensor 4 Error
    collector_max:
      name: Option Collector Max
    collector_min:
      name: Option Collector Min
    collector_frost:
      name: Option Collector Frost
    tube_collector:
      name: Option Tube Collector
    recooling:
      name: Option Recooling
    hqm:
      name: Option Heat Quantity Measurement
```

The `uart_id` is optional.

All sensors are optional.

For the custom sensor, all the parameters are optional and if not specified, will match any value.
```
