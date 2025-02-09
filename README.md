# Twingo Radio

Software for the Arduino managing the infotainment system in my Renault Twingo.

## Hardware setup
I replaced the factory FM-radio with an Android Auto setup.
The setup is powered by a raspberry pi 4 running Crankshaft-NG.
Together with an Arduino and four relays. While I eventually want to create
a PCB for this setup, it's currently hand-soldered underneath the driver's seat.

I used a tablet holder to bolt a 7.5" display to my dashboard.
Inside the old place for the radio is a USB hub for the touch screen, 
a USB microphone, an AA-Wireless dongle and a USB cable for wired AA.
This USB hub needs to be powered, otherwhise AA will be very unstable if used wired.

There are four relays:
- Car remote detect
- Amplifier
- Main power relay
- Secondary power relay

### Car remote detect
This relay simply steps down the car's 12V remote signal, which was originally connected to the radio,
to a 3.3V signal, which is fed into the Arduino

### Amplifier
Switches on the speaker amplifier

### Main power relay
Switched by the car's 12V remote signal, turns on the 12V supply to the DC-DC converter

### Secondary power relay
Switched by the Arduino, also turns on the 12V supply to the DC-DC converter.
Used to supply power a little bit longer than the car's power off. This avoids
a 'plop' sound on the speakers. The amplifier is turned off a little bit before the Arduino
turns off this relay

### IO with the Pi
- Volume up
- Volume down
- Track next
- Track prev
- Mute pin
- Pi ready pin

### Pi Ready pin
Activated by the Pi with a startup script. Informs the Arduino the system is booted and the
amplifier can be turned on.

## Integration with the steering wheel
The Twingo has some buttons on the steering wheel to control the volume, source, etc.
The controls are hooked up to the Arduino so it can be used with Android Auto.

# License

MIT or Apache-2.0, at your option