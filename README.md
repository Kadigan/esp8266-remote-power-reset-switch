
# A remote power/reset "button" based on an ESP8266
### This project was developed on NodeMCU v1.0 (lol1n board).


It's essentially what it says on the tin: a remote reset/power "button" you connect to your WiFi. Automatically reconnects to WiFi when connection is lost.

It exposes the following paths:

    / (+ any 404)             JSON info
    
    /reset                    reset switch (always short, 100ms "push")
    
    /power                    power switch, short pulse
    /power/short              (same)
    
    /power/long               power switch, hold (5s)

Lines used for the "switches" are D1 and D2.

Circuit essentials:

![ESP8266 switch](https://github.com/Kadigan/esp8266-remote-power-reset-switch/assets/16637976/61f10ace-5ef8-449d-afa2-13582a699e72)


The transoptors are wired in series; this should ensure that the polarity of the connection doesn't matter.

You can add a pin header to "passthru" your existing power/reset switches, if you want to.

Powering the ESP8266 up is entirely up to you - just keep in mind that if you power it from the PC you want to switch on, you'll want to hook into +5VSB so that it doesn't get killed with the rest of the system. ;)
