# component to read and control an ETA SH system

A configured uart is required.

Example:
```yaml
eta_sh:
  boiler_temperature:
    name: Kesseltemperatur
  return_temperature:
    name: Kesselruecklauf
  buffer_bottom_temperature:
    name: Puffer unten
  buffer_middle_temperature:
    name: Puffer mitte
  buffer_top_temperature:
    name: Puffer oben
  exhaust_temperature:
    name: Abgastemperatur
  room1_temperature:
    name: Raumtemperatur
  room1_output:
    name: Vorlauf Raum 1
  outside_temperature:
    name: Aussentemperatur
  buffer_load:
    name: Pufferladezustand
  external_heater_temperature:
    name: Externer Brenner
  
```

All sensors are optional.

