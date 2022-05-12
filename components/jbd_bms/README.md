# JBD BMS component

A configured uart component is required.
All sensors are optional.

Example:
```yaml
jbd_bms:
  - id: bms
    update_interval: 60s  # optional, default value
    voltage:
      name: Voltage
    current:
      name: Current
    capacity:
      name: Capacity
    battery_voltage:
      - string: 1   # 1-based
        name: String 1 voltage
    temperature:
      - number: 0   # 0-based
        name: NTC 0 temp
    balance_capacity:
      name: Balance Capacity
    rate_capacity:
      name: Rate Capacity
    charge_fet:
      name: Charge FET
    discharge_fet:
      name: Discharge FET
    covp_prot:
      name: COVP
    cuvp_prot:
      name: CUVP
    povp_prot:
      name: POVP
    puvp_prot:
      name: PUVP
    chgot_prot:
      name: CHGOT
    chgut_prot:
      name: CHGUT
    dsgot_prot:
      name: DSGOT
    dsgut_prot:
      name: DSGUT
    chgoc_prot:
      name: CHGOC
    dsgoc_prot:
      name: DSGOC
    short_prot:
      name: Short
    afe_prot:
      name: AFE
    swlock:
      name: SW Lock
    balance_state:
      - string: 1   # 1-based
        name: String 1 balancing
```

