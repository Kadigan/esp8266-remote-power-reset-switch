# A remote power/reset "button" based around an ESP8266
This project was developed on NodeMCU v1.0 (lol1n board).


It's essentially what it says on the tin: a remote reset/power "button" you connect to your WiFi.

It exposes the following paths:
/ (+ any 404) = JSON info
/reset = reset switch (always short, 100ms "push")
/power = power switch, short pulse
/power/long = power switch, hold (5s)

Lines used for the "switches" are D1 and D2.

Schematic & board: TBD
