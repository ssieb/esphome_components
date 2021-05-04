# arbitrary web handler

A component to run actions from arbitrary URLs.

NOTE:
Do NOT use `delay:` or any other action that might call it.  It WILL crash!

Example:
```yaml
web_handler:
  - path: /mine
    on_request:
      - lambda: stream.print("<h1>This is <b>MINE</b>!</h1>");
      - light.toggle: mylight
  - path: /yours
    mime_type: text/plain
    on_request:
      - lambda: stream.print("This is\nyours.");
```

