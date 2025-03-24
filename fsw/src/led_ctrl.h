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
**    Define GPIO Controller class
**
**  Notes:
**    TODO - Consider adding a map command if it fails during init. 
**
*/

#ifndef _led_ctrl_
#define _led_ctrl_

/*
** Includes
*/
#include "app_cfg.h"

/***********************/
/** Macro Definitions **/
/***********************/

/*
** Event Message IDs
*/
#define LED_CTRL_CONSTRUCTOR_EID  (LED_CTRL_BASE_EID + 0)
#define LED_CTRL_CHILD_TASK_EID   (LED_CTRL_BASE_EID + 1)

/**********************/
/** Type Definitions **/
/**********************/

/******************************************************************************
** GPIO_CTRL_Class
*/
typedef struct
{
   INITBL_Class_t  *IniTbl;
   bool    IsMapped;
   bool    LedOn;
   uint8   OutPin;
} LED_CTRL_Class_t;

/************************/
/** Exported Functions **/
/************************/

/******************************************************************************
** Function: LED_CTRL_Constructor
*/
void LED_CTRL_Constructor(LED_CTRL_Class_t *LedCtrlPtr, INITBL_Class_t *IniTbl);

/******************************************************************************
** Function: LED_CTRL_ChildTask
*/
bool LED_CTRL_ChildTask(CHILDMGR_Class_t *ChildMgr);

/******************************************************************************
** Function: LED_CTRL_ResetStatus
*/
void LED_CTRL_ResetStatus(void);

/******************************************************************************
** Function: LED_CTRL_TurnOnCmd
*/
bool LED_CTRL_TurnOnCmd(void *DataObjPtr, const CFE_MSG_Message_t *MsgPtr);

/******************************************************************************
** Function: LED_CTRL_TurnOffCmd
*/
bool LED_CTRL_TurnOffCmd(void *DataObjPtr, const CFE_MSG_Message_t *MsgPtr);

#endif /* _led_ctrl_ */
