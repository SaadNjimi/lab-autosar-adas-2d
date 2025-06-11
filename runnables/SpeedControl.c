#include <Rte_TargetSpeedControl.h>

#define MIN_TARGET_SPEED 10.0f       // Minimum speed in km/h
#define MAX_TARGET_SPEED 130.0f      // Maximum speed in km/h
#define SPEED_STEP 5.0f              // Step change in km/h

void TargetSpeedControl_SpeedControl(void)
{
    static float32 currentTargetSpeed = MIN_TARGET_SPEED;
    static uint8 lastButtonState = 0;

    uint8 buttonState = Rte_IRead_SpeedControl_TargetSpeedButtonState_Value();

    // Only act on new button press (rising edge detection)
    if (buttonState != lastButtonState) {
        switch (buttonState) {
            case 1: // Increase speed
                currentTargetSpeed += SPEED_STEP;
                if (currentTargetSpeed > MAX_TARGET_SPEED) {
                    currentTargetSpeed = MAX_TARGET_SPEED;
                }
                break;

            case 2: // Decrease speed
                currentTargetSpeed -= SPEED_STEP;
                if (currentTargetSpeed < MIN_TARGET_SPEED) {
                    currentTargetSpeed = MIN_TARGET_SPEED;
                }
                break;

            default:
                // No action
                break;
        }
    }

    lastButtonState = buttonState; // Store state for next call

    // Write the updated speed
    Rte_IWrite_SpeedControl_TargetSpeed_Value(currentTargetSpeed);
}