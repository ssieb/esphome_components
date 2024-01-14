# component to read and control an ETA SH system

A configured uart is required.

Example:
```yaml
eta_sh:
  boiler_temperature:
    name: Kesseltemperatur  (Boiler temp)
  fan_speed:
    name: Gebläsedrehzahl   (Speed of boiler Fan)
    unit_of_measurement: RPM
  return_temperature:
    name: Kesselruecklauf   (Boiler water return temp)
  buffer_bottom_temperature:
    name: Puffer unten      (Water buffer bottom temp)
  buffer_middle_temperature:
    name: Puffer mitte      (Water buffer middle temp)
  buffer_top_temperature:
    name: Puffer oben       (Water buffer top temp)
  exhaust_temperature:
    name: Abgastemperatur   (Exhaust temp)
  room1_temperature:
    name: Raumtemperatur    (Room1 temp)
  room1_output_temperature:
    name: Vorlauf Raum 1    (Water to room1 temp)
  outside_temperature:
    name: Aussentemperatur  (Outside temp)
  buffer_load:
    name: Pufferladezustand (Buffer load in %)
  external_heater_temperature:
    name: Externer Brenner  (External Oven (expansion, e.g. Oil or Gas)
  oxygen_sensor:
    name: Restsauerstoff   	(o2 Sensor Percent / Lambda Sond. Returns 0 when oven off)
  heater_status:
    name: "Status"                      Status of the heater,
                                        0: "Tür offen"			(Door open)
                                        1: "heizt an"			(starting Fire)
                                        2: "in Betrieb"			(working)
                                        3: "Übertemperatur"		(over temperature Limit)
                                        4: "Glutabbrand"		(embers burning)
                                        5: "Feuer aus"			(no Fire)
                                        6: "Ausgeschaltet"		(heater off)
                                        7: "Störung"			(fault)
```

All sensors are optional.

