# component that controls a PuroAir air purifier

Two configured uarts are required for full function.  One going to the button board and one to the control board.

Example:
```yaml
puroair:
  id: air
  button_uart: u1
  control_uart: u2
```

There is a method `press_button` that you can use in a lambda.

