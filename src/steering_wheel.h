#ifndef TWINGO_RADIO_STEERING_WHEEL_H
#define TWINGO_RADIO_STEERING_WHEEL_H

typedef enum SteeringWheelCommand {
    ST_VolUp,
    ST_VolDown,
    ST_Source,
    ST_Mute,
    ST_TrackForward,
    ST_TrackBack,
    ST_FolderForward,
    ST_FolderBack,
    ST_None
} SteeringWheelCommand;

SteeringWheelCommand getSteeringWheelInput();

#endif //TWINGO_RADIO_STEERING_WHEEL_H
