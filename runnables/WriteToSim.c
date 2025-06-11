#include <Rte_ActuatorSWC.h>
#include <Sab.h>
#include <Sab_Types.h>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define TX_PIPE_NAME "\\\\.\\pipe\\veos_pipe_in"

#define PIPE_READMODE_MESSAGE 0x00000002
#define PIPE_NOWAIT 0x00000001

#define ERROR_NO_DATA 0xE8
#define ERROR_REMOTE_GONE 0x109

#define MSG_LEN 6
#define BUF_SIZE MSG_LEN*2

#define SIG_IN_COUNTER_IDX 0
#define SIG_OUT_COUNTER_IDX 0
#define SIG_OUT_BOOLS_IDX 1
#define SIG_OUT_TARGET_SPEED_IDX 2

#define SIG_BOOL_ACCELERATE_MASK        0b0001
#define SIG_BOOL_BRAKE_MASK             0b0010
#define SIG_BOOL_EMERGENCY_BRAKE_MASK   0b0100
#define SIG_BOOL_NEW_CC_STATE_MASK      0b1000

int get_tx_pipe()
{
   int ph;
   ph = open(TX_PIPE_NAME, O_WRONLY | O_BINARY);

   if (ph == -1) {
       Sab_SubmitInfo("Failed to open tx pipe");
       return -1;
   } else {
        Sab_SubmitInfo("CONNECTED to tx pipe");
        return ph;
   }
}

void ActuatorSWC_WriteToSim()
{
	static int tx = -1;	
	
	if (tx == -1)
	{		
		tx = get_tx_pipe();
	}

	if (tx != -1)
	{
		uint8 counter = Rte_IRead_WriteToSim_Counter_Value();
		counter++;
		
		char txBuf[MSG_LEN];
		
		txBuf[SIG_OUT_COUNTER_IDX] = counter;
		txBuf[SIG_OUT_BOOLS_IDX] = 0;
		if (Rte_IRead_WriteToSim_CC_StateNew_Value()) {
			txBuf[SIG_OUT_BOOLS_IDX] |= SIG_BOOL_NEW_CC_STATE_MASK;
		}
		if (Rte_IRead_WriteToSim_Accelerate_Value()) {
			txBuf[SIG_OUT_BOOLS_IDX] |= SIG_BOOL_ACCELERATE_MASK;
		}
		if (Rte_IRead_WriteToSim_Brake_Value()) {
			txBuf[SIG_OUT_BOOLS_IDX] |= SIG_BOOL_BRAKE_MASK;
		}
		if (Rte_IRead_WriteToSim_EmergencyBrake_Value()) {
			txBuf[SIG_OUT_BOOLS_IDX] |= SIG_BOOL_EMERGENCY_BRAKE_MASK;
		}

		uint32 target_speed = (uint32)Rte_IRead_WriteToSim_TargetSpeed_Value();
		if (target_speed > 255) {
			target_speed = 255;
		}
		txBuf[SIG_OUT_TARGET_SPEED_IDX] = (uint8)target_speed;

		int sent = write(tx, txBuf, MSG_LEN);		
	}
}