# sen0377 component

This component provides support for the [DFRobot SEN0377 MEMS gas sensor](https://www.dfrobot.com/product-2417.html)

You need an `i2c:` component configured.  All the sensors are optional.

Example:
```yaml
sensor:
  - platform: sen0377
    co:
      name: CO
    methane:
      name: Methane
    ethanol:
      name: Ethanol
    hydrogen:
      name: Hydrogen
    ammonia:
      name: Ammonia
    no2:
      name: NO2
    detect:      # binary detection sensors
      co:
        name: Detect CO
      methane:
        name: Detect Methane
      ethanol:
        name: Detect Ethanol
      propane:
        name: Detect Propane
      isobutane:
        name: Detect Isobutane
      hydrogen:
        name: Detect Hydrogen
      h2s:
        name: Detect H2S
      ammonia:
        name: Detect Ammonia
      no1:
        name: Detect NO
      no2:
        name: Detect NO2
```
