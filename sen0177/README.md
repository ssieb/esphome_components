# SEN0177 Air Quality Sensor

Example:
```yaml
uart:
  id: aq_uart
  rx_pin: D7
  baud_rate: 9600

sensor:
  - platform: sen0177
    uart_id: aq_uart
    pm_1_0:
      name: "Particulate Matter <1.0µm Concentration"
    pm_2_5:
      name: "Particulate Matter <2.5µm Concentration"
    pm_10_0:
      name: "Particulate Matter <10.0µm Concentration"
    pmc_0_3:
      name: "Particulate Matter >0.3µm Count"
    pmc_0_5:
      name: "Particulate Matter >0.5µm Count"
    pmc_1_0:
      name: "Particulate Matter >1.0µm Count"
    pmc_2_5:
      name: "Particulate Matter >2.5µm Count"
    pmc_5_0:
      name: "Particulate Matter >5.0µm Count"
    pmc_10_0:
      name: "Particulate Matter >10.0µm Count"
```

