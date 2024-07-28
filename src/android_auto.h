//
// Created by tobias on 28-7-24.
//

#ifndef TWINGO_RADIO_ANDROID_AUTO_H
#define TWINGO_RADIO_ANDROID_AUTO_H

#include "steering_wheel.h"

typedef enum AACommand {
    AA_VOL_UP,
    AA_VOL_DOWN,
    AA_TRACK_NEXT,
    AA_TRACK_PREV,
    AA_MUTE
} AACommand;

void triggerAACommand(AACommand inputType);

bool AAInputFromSteeringWheelCommand(SteeringWheelCommand cmd, AACommand *out);

#endif //TWINGO_RADIO_ANDROID_AUTO_H
