#include "ClockSwitch.h"

// Constructor to initialize the settings struct
ClockSwitch::ClockSwitch(Settings* settingsStruct) {
    settings = settingsStruct;
    lowerHour = 0;
    lowerMinute = 0;
    upperHour = 0;
    upperMinute = 0;
    outputState = false;
}

// Function to set the time range
void ClockSwitch::SetTime(int lowerHour, int lowerMinute, int upperHour, int upperMinute) {
    this->lowerHour = lowerHour;
    this->lowerMinute = lowerMinute;
    this->upperHour = upperHour;
    this->upperMinute = upperMinute;
}

// Function to check if the current time is between the lower and upper times, accounting for passing midnight
bool ClockSwitch::isTimeInRange(int currentTimeInSeconds, int lowerTimeInSeconds, int upperTimeInSeconds) {
    if (lowerTimeInSeconds <= upperTimeInSeconds) {
        // Normal case, the range doesn't pass midnight
        return (currentTimeInSeconds >= lowerTimeInSeconds && currentTimeInSeconds <= upperTimeInSeconds);
    } else {
        // Special case where the time range passes over midnight
        return (currentTimeInSeconds >= lowerTimeInSeconds || currentTimeInSeconds <= upperTimeInSeconds);
    }
}

// Function to update the output based on the current time
void ClockSwitch::update() {
    int currentTimeInSeconds = settings->hours * 3600 + settings->minutes * 60 + settings->seconds;
    int lowerTimeInSeconds = lowerHour * 3600 + lowerMinute * 60;
    int upperTimeInSeconds = upperHour * 3600 + upperMinute * 60;

    // Set the output high if the current time is in the range (accounting for passing midnight)
    if (isTimeInRange(currentTimeInSeconds, lowerTimeInSeconds, upperTimeInSeconds)) {
        outputState = true;
    } else {
        outputState = false;
    }
}

// Function to get the output state
bool ClockSwitch::getOutputState() {
    return outputState;
}