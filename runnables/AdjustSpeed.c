
#include <Rte_CruiseControl.h>
#include <Sab.h>
#define target_speed 50

float32 Input_Speed, Input_TargetSpeed;
boolean Accelerate, Brake, CC_State;


void CruiseControl_AdjustSpeed()
    {
    CC_State = Rte_IRead_AdjustSpeed_CC_State_Value(); 
    Input_Speed= Rte_IRead_AdjustSpeed_CurrentSpeed_Value();
    Input_TargetSpeed= Rte_IRead_AdjustSpeed_TargetSpeed_Value();

        /* Read the Pedal State*/
    boolean accel_pedal = Rte_IRead_AdjustSpeed_AcceleratorPedal_Value();
    boolean brake_pedal = Rte_IRead_AdjustSpeed_BrakePedal_Value(); 
    if(!CC_State){

        Rte_IWrite_AdjustSpeed_CC_StateNew_Value(0);
         // CC is OFF - direct pedal pass-through
        Rte_IWrite_AdjustSpeed_Accelerate_Value(accel_pedal);
        Rte_IWrite_AdjustSpeed_Brake_Value(brake_pedal); 
    }else{ 
        // Brake pedal pressed - disable CC immediately
        if(brake_pedal)
        { 
            Rte_IWrite_AdjustSpeed_CC_StateNew_Value(0);
            Rte_IWrite_AdjustSpeed_Accelerate_Value(0);
            Rte_IWrite_AdjustSpeed_Brake_Value(1);
        }  else if(accel_pedal) {
            // Accelerator override - let driver go beyond target speed
            Rte_IWrite_AdjustSpeed_CC_StateNew_Value(1);
            Rte_IWrite_AdjustSpeed_Accelerate_Value(1);
            Rte_IWrite_AdjustSpeed_Brake_Value(0);
        }  else {
            
            // Normal CC operation - maintain target speed
            if(Input_Speed > Input_TargetSpeed)
            {
                Rte_IWrite_AdjustSpeed_Accelerate_Value(0);
                Rte_IWrite_AdjustSpeed_Brake_Value(1);
            }
            else if(Input_Speed < Input_TargetSpeed)
            {
                Rte_IWrite_AdjustSpeed_Accelerate_Value(1);
                Rte_IWrite_AdjustSpeed_Brake_Value(0);
            }
            Rte_IWrite_AdjustSpeed_CC_StateNew_Value(1);
        } 

    }
} 