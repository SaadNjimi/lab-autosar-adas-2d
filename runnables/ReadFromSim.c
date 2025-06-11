#include <Rte_SensorSWC.h>
#include <Sab.h>
#include <Sab_Types.h>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define RX_PIPE_NAME "\\\\.\\pipe\\veos_pipe_out"

#define PIPE_READMODE_MESSAGE 0x00000002
#define PIPE_NOWAIT 0x00000001

#define ERROR_NO_DATA 0xE8
#define ERROR_REMOTE_GONE 0x109

#define MSG_LEN 6
#define BUF_SIZE MSG_LEN*2

#define SIG_IN_COUNTER_IDX 0
#define SIG_IN_SPEED_INT_IDX 1
#define SIG_IN_SPEED_FRAC_IDX 2
#define SIG_IN_BOOLS_IDX 3
#define SIG_IN_DISTANCE_INT_IDX 4
#define SIG_IN_DISTANCE_FRAC_IDX 5

#define SIG_BOOL_ACCEL_PEDAL_MASK      0b00000100
#define SIG_BOOL_BRAKE_PEDAL_MASK      0b00001000
#define SIG_BOOL_CC_STATE_MASK         0b00010000
#define SIG_BOOL_TARGETSPEED_BTN_MASK  0b00000011

// manually expanded winapi import, since we cannot include <windows.h>
__declspec (dllimport) int __stdcall SetNamedPipeHandleState (void * hNamedPipe, unsigned long *lpMode, unsigned long *lpMaxCollectionCount, unsigned long *lpCollectDataTimeout);

void set_pipe_mode(int ph, unsigned long mode)
{
    void* os_handle = (void*)_get_osfhandle(ph);

    unsigned long dwMode = mode;
    if (!SetNamedPipeHandleState(os_handle, &dwMode, 0, 0))
    {
        Sab_SubmitInfo("FAILED to set NOWAIT");
    }
}

int get_rx_pipe()
{
    int ph = open(RX_PIPE_NAME, O_RDWR | O_BINARY);
    if (ph == -1)
    {
        Sab_SubmitInfo("Failed to open rx pipe");
        return -1;
    } else {
        Sab_SubmitInfo("CONNECTED to rx pipe");
        set_pipe_mode(ph, PIPE_NOWAIT | PIPE_READMODE_MESSAGE);
        return ph;
    }
}

void SensorSWC_ReadFromSim()
{
	static int rx = -1;
	
	if (rx == -1)
	{		
		rx = get_rx_pipe();
	}

	if (rx != -1)
	{
	    char rxBuf[BUF_SIZE];
		int rcvd = read(rx, rxBuf, MSG_LEN);
		if (rcvd > 0)
		{
				if (rcvd != MSG_LEN)
				{
					Sab_SubmitInfo("Message length mismatch!");
				}

				uint8 counter = rxBuf[SIG_IN_COUNTER_IDX];
				uint8 speed_int = rxBuf[SIG_IN_SPEED_INT_IDX];
				uint8 speed_frac = rxBuf[SIG_IN_SPEED_FRAC_IDX];
				float32 speed = speed_int + speed_frac / 100.0f;

				boolean cc = (rxBuf[SIG_IN_BOOLS_IDX] & SIG_BOOL_CC_STATE_MASK) == SIG_BOOL_CC_STATE_MASK;
				boolean accel_pedal = (rxBuf[SIG_IN_BOOLS_IDX] & SIG_BOOL_ACCEL_PEDAL_MASK) == SIG_BOOL_ACCEL_PEDAL_MASK;
				boolean brake_pedal = (rxBuf[SIG_IN_BOOLS_IDX] & SIG_BOOL_BRAKE_PEDAL_MASK) == SIG_BOOL_BRAKE_PEDAL_MASK;
				uint8 targetspeed_btn = rxBuf[SIG_IN_BOOLS_IDX] & SIG_BOOL_TARGETSPEED_BTN_MASK;
				
				uint8 distance_int = rxBuf[SIG_IN_DISTANCE_INT_IDX];
				uint8 distance_frac = rxBuf[SIG_IN_DISTANCE_FRAC_IDX];
				float32 distance = distance_int + distance_frac / 100.0f;
				
				Rte_IWrite_ReadFromSim_Counter_Value(counter);								
				Rte_IWrite_ReadFromSim_CurrentSpeed_Value(speed);			
				Rte_IWrite_ReadFromSim_CC_State_Value(cc);					
				Rte_IWrite_ReadFromSim_AcceleratorPedal_Value(accel_pedal);	
				Rte_IWrite_ReadFromSim_BrakePedal_Value(brake_pedal);					
				
				Rte_IWrite_ReadFromSim_TargetSpeedButtonState_Value(targetspeed_btn);	
				Rte_IWrite_ReadFromSim_DistanceLeading_Value(distance);	
		}
	}
}