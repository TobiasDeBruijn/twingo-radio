//
// Created by tobias on 24-8-23.
//

#ifndef TWINGO_RADIO_MAIN_H
#define TWINGO_RADIO_MAIN_H

// Input pins
#define BLACKPIN 2
#define BLUEPIN 3
#define REDPIN 4
#define GREENPIN 5
#define YELLOWPIN 6
#define BROWNPIN 7

#define OUT_PINS 3
unsigned char out_pins[OUT_PINS] = {GREENPIN, BLUEPIN, YELLOWPIN};

// Output pins
#define VOL_DOWN_PIN 8
#define VOL_UP_PIN 9
#define LEDPIN 13

enum InputCommand {
    VolUp,
    VolDown,
    Source,
    Mute,
    TrackForward,
    TrackBack,
    FolderForward,
    FolderBack,
    None
};

#endif //TWINGO_RADIO_MAIN_H