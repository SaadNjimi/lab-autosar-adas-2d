#include <Rte_EmergencyBrake.h>
#include <Sab.h>  // For debug output

// Constants
#define EMERGENCY_DECELERATION 14.0f      // m/s²
#define STOPPED_THRESHOLD       0.1f      // m/s: considered stopped
#define SAFETY_MARGIN           2.0f      // m: added buffer to avoid crash
#define KMH_TO_MS               (1.0f / 3.6f)  // Convert km/h to m/s

void EmergencyBrake_EmergencyBrake(void)
{
    static boolean emergencyActive = FALSE;

    // Read inputs
    float32 currentSpeedKmh = Rte_IRead_EmergencyBrake_CurrentSpeed_Value();       // km/h
    float32 distanceLeading = Rte_IRead_EmergencyBrake_DistanceLeading_Value();    // meters

    // Convert speed to m/s
    float32 currentSpeedMs = currentSpeedKmh * KMH_TO_MS;

    if (emergencyActive)
    {
        // Already braking — check if the car has stopped
        if (currentSpeedMs <= STOPPED_THRESHOLD)
        {
            emergencyActive = FALSE;
            Rte_IWrite_EmergencyBrake_EmergencyBrake_Value(FALSE); // Stop braking
            Sab_SubmitInfo("Emergency brake released - vehicle stopped");
        }
    }
    else
    {
        // Only evaluate if there's a leading vehicle
        if (distanceLeading > 0.0f)
        {
            // Compute emergency braking distance
            float32 brakingDistance = (currentSpeedMs * currentSpeedMs) / (2.0f * EMERGENCY_DECELERATION);
            float32 triggerDistance = brakingDistance + SAFETY_MARGIN;

            // Debug print (optional)
            Sab_SubmitInfo("Speed: %.2f m/s | LeadDist: %.2f m | BrakeDist: %.2f m",
                           currentSpeedMs, distanceLeading, brakingDistance);

            // Trigger emergency brake early enough
            if (distanceLeading <= triggerDistance)
            {
                emergencyActive = TRUE;
                Rte_IWrite_EmergencyBrake_CC_StateNew_Value(FALSE);  // Disable Cruise Control
                Rte_IWrite_EmergencyBrake_EmergencyBrake_Value(TRUE); // Trigger brake
                Sab_SubmitInfo("Emergency brake triggered!");
            }
        }
    }
}