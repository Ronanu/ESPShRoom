#include "ClockSwitch.h"

// Constructor to initialize the settings struct
ClockSwitch::ClockSwitch(Settings* settingsStruct) {
    settings = settingsStruct;
    outputState = false;
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
    int lowerTimeInSeconds = settings->ct_lowerHour * 3600 + settings->ct_lowerMinute * 60;
    int upperTimeInSeconds = settings->ct_upperHour * 3600 + settings->ct_upperMinute * 60;

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