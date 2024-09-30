#ifndef CLOCKSWITCH_H
#define CLOCKSWITCH_H

#include "Settings.h" // Assuming the Settings struct is in a separate file

class ClockSwitch {
private:
    Settings* settings;
    int lowerHour, lowerMinute, upperHour, upperMinute;
    bool outputState;

public:
    // Constructor to initialize the settings struct
    ClockSwitch(Settings* settingsStruct);

    // Function to set the time range
    void SetTime(int lowerHour = 0, int lowerMinute = 0, int upperHour = 0, int upperMinute = 0);

    // Function to update the output based on the current time
    void update();

    // Function to get the output state
    bool getOutputState();

    // Function to check if the current time is between the lower and upper times, accounting for passing midnight
    bool isTimeInRange(int currentTimeInSeconds, int lowerTimeInSeconds, int upperTimeInSeconds);
};

#endif // CLOCKSWITCH_H