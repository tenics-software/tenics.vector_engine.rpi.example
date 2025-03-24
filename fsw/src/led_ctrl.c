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

static LED_CTRL_Class_t  *LedCtrl = NULL;

/******************************************************************************
** Function: LED_CTRL_Constructor
*/
void LED_CTRL_Constructor(LED_CTRL_Class_t *LedCtrlPtr, INITBL_Class_t *IniTbl)
{
   LedCtrl = LedCtrlPtr;
   memset(LedCtrl, 0, sizeof(LED_CTRL_Class_t));
   LedCtrl->OutPin = INITBL_GetIntConfig(IniTbl, CFG_CTRL_OUT_PIN);

   if (gpio_map() < 0) // map peripherals
   {
      CFE_EVS_SendEvent(LED_CTRL_CONSTRUCTOR_EID, CFE_EVS_EventType_ERROR, 
                        "GPIO map failed. Verify rpi_iolib's config.h BCM setting and run with elevated privileges.");
      LedCtrl->IsMapped = false;
   }
   else
   {
      LedCtrl->IsMapped = true;  
      gpio_out(LedCtrl->OutPin);
   }
}

/******************************************************************************
** Function: LED_CTRL_TurnOnCmd
*/
bool LED_CTRL_TurnOnCmd(void *DataObjPtr, const CFE_MSG_Message_t *MsgPtr)
{
   if (LedCtrl->IsMapped)
   {
      gpio_set(LedCtrl->OutPin);
      LedCtrl->LedOn = true;
      CFE_EVS_SendEvent(LED_CTRL_CHILD_TASK_EID, CFE_EVS_EventType_INFORMATION, 
                        "GPIO pin %d turned ON", LedCtrl->OutPin);
   }
   return true;
}

/******************************************************************************
** Function: LED_CTRL_TurnOffCmd
*/
bool LED_CTRL_TurnOffCmd(void *DataObjPtr, const CFE_MSG_Message_t *MsgPtr)
{
   if (LedCtrl->IsMapped)
   {
      gpio_clr(LedCtrl->OutPin);
      LedCtrl->LedOn = false;
      CFE_EVS_SendEvent(LED_CTRL_CHILD_TASK_EID, CFE_EVS_EventType_INFORMATION, 
                        "GPIO pin %d turned OFF", LedCtrl->OutPin);
   }
   return true;
}

/******************************************************************************
** Function: LED_CTRL_ChildTask
*/
bool LED_CTRL_ChildTask(CHILDMGR_Class_t* ChildMgr)
{
   return false; // No need for continuous looping as commands handle on/off
}

/******************************************************************************
** Function: LED_CTRL_ResetStatus
*/
void LED_CTRL_ResetStatus(void)
{
   return;
}
