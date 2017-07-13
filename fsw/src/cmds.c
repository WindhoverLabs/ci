/*==============================================================================
Copyright (c) 2015, Windhover Labs
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of CmdIn nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef __cplusplus
extern "C" {
#endif

/*
** Pragmas
*/

/*
** Include Files
*/
#include "ci_mission_cfg.h"
#include "app.h"
#include "cmds.h"
#include "events.h"
#include "cmd_codes.h"
#include "tbl.h"



/*
** Local Defines
*/

/*
** Local Structure Declarations
*/



/*
** External Global Variables
*/
extern CI_AppData_T CI_AppData;



/*
** Global Variables
*/

/*
** Local Variables
*/

/*
** Local Function Prototypes
*/



void CI_CmdExec(CFE_SB_MsgPtr_t msg)
{
    uint16 CommandCode;
    boolean bValid = TRUE;

    /* Get Command Code */
    CommandCode = CFE_SB_GetCmdCode(msg);

    switch (CommandCode)
    {
       case CI_NOP_CC:
       {
            CFE_EVS_SendEvent(CI_NOOP_INF_EID, CFE_EVS_INFORMATION,
            		"CI: No-op Command Rcvd");
            break;
       }

       case CI_RESET_STATUS_CC:
       {
            CI_ResetStatus();
            break;
       }

       case CI_SET_SCHED_MODE_CC:
       {
    	   CI_SetSchedMode_Msg_t *modeMsg = (CI_SetSchedMode_Msg_t*)msg;

    	   bValid = CI_SetSchedMode(modeMsg->Mode);

   	    break;
       }

       case CI_CMD_ARM:
       {
    	    //CI_ArmDisarm_Msg_t *armMsg = (CI_ArmDisarm_Msg_t*)msg;

    	    //bValid = CI_CmdArm(armMsg->MsgID, armMsg->CmdCode);

    	    break;
       }

       case CI_CMD_DISARM:
       {
    	    //CI_ArmDisarm_Msg_t *armMsg = (CI_ArmDisarm_Msg_t*)msg;

    	    //bValid = CI_CmdDisarm(armMsg->MsgID, armMsg->CmdCode);

    	    break;
       }

       default:
       {
            CFE_EVS_SendEvent(CI_FNCODE_ERR_EID,CFE_EVS_ERROR,
            		  "CI: Invalid Function Code Rcvd In Ground Command 0x%x",
                      CommandCode);
            bValid = FALSE;
       }
    }

    if (bValid)
    {
       CI_AppData.HkPacket.CmdCount++;
    }
    else
    {
       CI_AppData.HkPacket.CmdErrCount++;
    }
}



void CI_ResetStatus(void)
{
	CI_AppData.HkPacket.CmdCount = -1;
	CI_AppData.HkPacket.CmdErrCount = 0;
    CI_AppData.HkPacket.IngestMsgCount = 0;
    CI_AppData.HkPacket.IngestErrorCount = 0;
    CI_AppData.HkPacket.FDPdusDropped = 0;
    CI_AppData.HkPacket.EOFPdusDropped = 0;
    CI_AppData.HkPacket.FINPdusDropped = 0;
    CI_AppData.HkPacket.ACKPdusDropped = 0;
    CI_AppData.HkPacket.MDPdusDropped = 0;
    CI_AppData.HkPacket.NAKPdusDropped = 0;
    CI_AppData.HkPacket.PDUsCaptured = 0;
}



boolean CI_SetSchedMode(CI_SchedMode_t Mode)
{
	if(Mode == CI_SCHED_MODE_ASYNCHRONOUS)
	{
		if(CI_AppData.HkPacket.SchedMode == CI_SCHED_MODE_SYNCHRONOUS)
		{
			/* Start the listener thread with blocking read. */
			CI_AppData.HkPacket.SchedMode = CI_SCHED_MODE_ASYNCHRONOUS;
			return TRUE;
		}
		else
		{
			return FALSE;
			/* Reject command.  We're already in event driven mode. */
		}
	}
	else
	{
		if(CI_AppData.HkPacket.SchedMode == CI_SCHED_MODE_ASYNCHRONOUS)
		{
			/* Kill the listener thread. */
			CI_AppData.HkPacket.SchedMode == CI_SCHED_MODE_SYNCHRONOUS;
			return TRUE;
		}
		else
		{
			/* Reject command.  We're already in scheduled mode. */
			return FALSE;
		}
	}
}



#ifdef __cplusplus
}
#endif
