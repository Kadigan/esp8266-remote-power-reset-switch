# A remote power/reset "button" based on an ESP8266
## It's essentially what it says on the tin: a remote reset/power "button" you connect to your WiFi. Automatically reconnects to WiFi when connection is lost.
### This project was developed on NodeMCU v1.0 (lol1n board).

The transoptors are wired in series; this **should** ensure that the polarity of the connection doesn't matter - but *in case that doesn't work* or that you see weird effects like PWR influencing RST (some boards may be more sensitive to this than others), your board's manual should tell you the correct polarity to use (if the board itself doesn't have it printed on the silkscreen - they often do).

**If you decide to use relays instead** (to ensure maximum electrical separation) - that's also doable, but you'll want to use a driver like a ULN2003 or something similar (ULN2003 is rated at 500 mA per output; if your coil requires more, wire a pair of outputs per coil – it's as simple as shorting adjacent inputs together, and adjacent outputs together). ULN2003 has 7 of these drivers and comes equipped with the reverse diodes for spike dissipation, so it's a single-chip solution. Not sure if a pull-down is necessary in this situation (or even for this board).

You can add a pin header to "passthru" your existing power/reset switches if you want to, regardless of the solution you pick. It's what I did.

I would also recommend *against* using D0 for any outputs, because it seems to fluctuate on board start-up - it does *something* on that pin, and I didn't care enough to find out *what*. Don't use it if you don't want spurious activations on ESP boot.

**Powering the ESP8266 up is entirely up to you** – just keep in mind that if you power it from the PC you want to switch on, you'll want to hook into the +5VSB rail, so that it doesn't get shut off with the rest of the system. Also keep in mind that +5VSB is _not guaranteed_ to come back up on mains restore – my power supplies (the ones where I cared to check, at least) usually _didn't_ bring it back up, and required _at least one physical power-up_ to restore its presence. _Something to keep in mind._ I powered my board using an external mains power supply.

**The board is configured for power-button long-hold at 5 seconds.** For modern boards, soft-off is usually at 4 seconds; older boards may require the full 8 second hold (would recommend 9) - adjust as necessary, it should be hard-coded into `actions.cpp`.

**If you want to use this with an older, AT board** – one that requires a physical switch to be turned on or off – then I would recommend a bi-stable coil on a pass-through mains voltage line instead. A generic relay will work just as well, but if you don't want the headache of having to keep a coil powered for extended periods of time, it's what I'd recommend. I used an RT424FO5 in another project that switches mains on and off, because it can run off of 5V (so I could have a common supply for it _and_ the board), has a low DC set voltage if required, is rated at up to 8A at 250V AC, and can go up to 400V AC if required. _Your mileage may vary._ Remember to use a coil that is rated _at mains voltage_ for your area, and size it up to your load (typ. PC loads are around the 800W mark nowadays).

---
The code exposes the following paths:

    / (+ any 404)             JSON info
    
    /reset                    reset switch (always short, 100ms "push")
    
    /power                    power switch, short pulse
    /power/short              (same)
    
    /power/long               power switch, hold (5s)

Lines used for the "switches" are D1 and D2.

---
Circuit essentials:

![ESP8266 switch](https://github.com/Kadigan/esp8266-remote-power-reset-switch/assets/16637976/61f10ace-5ef8-449d-afa2-13582a699e72)
