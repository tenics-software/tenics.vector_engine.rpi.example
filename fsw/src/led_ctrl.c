/*
**  Copyright 2022 bitValence, Inc.
**  All Rights Reserved.
**
**  This program is free software; you can modify and/or redistribute it
**  under the terms of the GNU Affero General Public License
**  as published by the Free Software Foundation; version 3 with
**  attribution addendums as found in the LICENSE.txt
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU Affero General Public License for more details.
**
**  Purpose:
**    Implement the GPIO Controller Class methods
**
**  Notes:
**    1. A GPIO mapping failure is unrecoverable so the child task
**       exits. A mapping failure is most likely due to an incorrect
**       configuration in RPI_IOLIB's config.h file.  
**
*/

/*
** Include Files:
*/

#include <string.h>

#include "app_cfg.h"
#include "led_ctrl.h"
#include "gpio.h"

/**********************/
/** Global File Data **/
/**********************/

static GPIO_CTRL_Class_t  *GpioCtrl = NULL;


/*******************************/
/** Local Function Prototypes **/
/*******************************/


/******************************************************************************
** Function: GPIO_CTRL_Constructor
**
** Initialize the GPIO Controller object to a known state
**
** Notes:
**   1. This must be called prior to any other function.
**
*/
void GPIO_CTRL_Constructor(GPIO_CTRL_Class_t *GpioCtrlPtr, INITBL_Class_t *IniTbl)
{
   
   GpioCtrl = GpioCtrlPtr;
   
   memset(GpioCtrl, 0, sizeof(GPIO_CTRL_Class_t));
   GpioCtrl->OutPin  = INITBL_GetIntConfig(IniTbl, CFG_CTRL_OUT_PIN);
   GpioCtrl->OnTime  = INITBL_GetIntConfig(IniTbl, CFG_CTRL_ON_TIME);
   GpioCtrl->OffTime = INITBL_GetIntConfig(IniTbl, CFG_CTRL_OFF_TIME);
   
   if (gpio_map() < 0) // map peripherals
   {
   
      CFE_EVS_SendEvent(GPIO_CTRL_CONSTRUCTOR_EID, CFE_EVS_EventType_ERROR, "GPIO map failed. Verify rpi_iolib's config.h BCM setting and you are running with elevated privileges");
      GpioCtrl->IsMapped = false;

   }
   else
   {
      GpioCtrl->IsMapped = true;  
      gpio_out(GpioCtrl->OutPin);
   }
   
   
} /* End GPIO_CTRL_Constructor() */


/******************************************************************************
** Function: GPIO_CTRL_ChildTask
**
** Notes:
**   1. Returning false causes the child task to terminate.
**   2. Information events are sent because this is instructional code and the
**      events provide feedback. The events are filtered so they won't flood
**      the ground. A reset app command resets the event filter.  
**
*/
bool GPIO_CTRL_ChildTask(CHILDMGR_Class_t* ChildMgr)
{
   
   bool RetStatus = false;
   
   if (GpioCtrl->IsMapped)
   {
      
      gpio_set(GpioCtrl->OutPin);
      GpioCtrl->LedOn = true;
      CFE_EVS_SendEvent(GPIO_CTRL_CHILD_TASK_EID, CFE_EVS_EventType_INFORMATION, 
                        "GPIO pin %d on for %u milliseconds", GpioCtrl->OutPin, GpioCtrl->OnTime);
      OS_TaskDelay(GpioCtrl->OnTime);
    
      gpio_clr(GpioCtrl->OutPin);
      GpioCtrl->LedOn = false;
      CFE_EVS_SendEvent(GPIO_CTRL_CHILD_TASK_EID, CFE_EVS_EventType_INFORMATION, 
                        "GPIO pin %d off for %u milliseconds", GpioCtrl->OutPin, GpioCtrl->OffTime);
      OS_TaskDelay(GpioCtrl->OffTime);
   
      RetStatus = true;
   
   } /* End if mapped */
   
   return RetStatus;

} /* End GPIO_CTRL_ChildTask() */


/******************************************************************************
** Function: GPIO_CTRL_ResetStatus
**
** Reset counters and status flags to a known reset state.
**
** Notes:
**   1. Any counter or variable that is reported in HK telemetry that doesn't
**      change the functional behavior should be reset.
**
*/
void GPIO_CTRL_ResetStatus(void)
{

   return;

} /* End GPIO_CTRL_ResetStatus() */


/******************************************************************************
** Function: GPIO_CTRL_SetOffTimeCmd
**
** Notes:
**   1. No limits placed on commanded value.
**
*/
bool GPIO_CTRL_SetOffTimeCmd(void* DataObjPtr, const CFE_MSG_Message_t *MsgPtr)
{
   
   const RPI_LED_SetOffTime_Payload_t *Cmd = CMDMGR_PAYLOAD_PTR(MsgPtr, RPI_LED_SetOffTime_t);
   bool RetStatus = true;
  
   GpioCtrl->OffTime = Cmd->OffTime;  
   CFE_EVS_SendEvent(GPIO_CTRL_SET_OFF_TIME_EID, CFE_EVS_EventType_INFORMATION, "GPIO off time set to %u milliseconds", GpioCtrl->OffTime);
  
   return RetStatus;   
   
} /* End GPIO_CTRL_SetOffTimeCmd() */


/******************************************************************************
** Function: GPIO_CTRL_SetOnTimeCmd
**
** Notes:
**   1. No limits placed on commanded value.
**
*/
bool GPIO_CTRL_SetOnTimeCmd(void* DataObjPtr, const CFE_MSG_Message_t *MsgPtr)
{
   
   const RPI_LED_SetOnTime_Payload_t *Cmd = CMDMGR_PAYLOAD_PTR(MsgPtr, RPI_LED_SetOnTime_t);
   bool RetStatus = true;
  
   GpioCtrl->OnTime = Cmd->OnTime;
   CFE_EVS_SendEvent(GPIO_CTRL_SET_ON_TIME_EID, CFE_EVS_EventType_INFORMATION, "GPIO on time set to %u milliseconds", GpioCtrl->OnTime);
  
   return RetStatus;   
   
} /* End GPIO_CTRL_SetOnTimeCmd() */

