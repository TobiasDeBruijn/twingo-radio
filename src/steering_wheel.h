#ifndef TWINGO_RADIO_STEERING_WHEEL_H
#define TWINGO_RADIO_STEERING_WHEEL_H

typedef enum InputCommand {
    VolUp,
    VolDown,
    Source,
    Mute,
    TrackForward,
    TrackBack,
    FolderForward,
    FolderBack,
    None
} InputCommand;

InputCommand getSteeringWheelInput();

#endif //TWINGO_RADIO_STEERING_WHEEL_H
