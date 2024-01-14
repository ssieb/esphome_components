# esphome_components
My collection of components for esphome

To use these components, use the [external components](https://esphome.io/components/external_components.html).

Example:
```yaml
external_components:
  - source:
      type: git
      url: https://github.com/ssieb/esphome_components
    components: [ component1, component2 ]
```

## NOTE: Some components were merged to esphome :tada:
- [keypad](https://github.com/ssieb/custom_components/tree/master/components/keypad) -> [matrix_keypad](https://esphome.io/components/matrix_keypad.html?highlight=keypad)
