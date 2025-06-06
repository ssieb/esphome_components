# delay boot for a period of time

A component to make the boot wait at a certain priority.
Can be used multiple times.

Example:
```yaml
boot_delay:
  delay: 5s   # the amount of time since the boot started
  setup_priority: 600
```

