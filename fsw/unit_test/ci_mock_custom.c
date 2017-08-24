#include "ci_mock_custom.h"

int INIT_CUSTOM_RET;
int READ_MSG_RET;

uint8 ci_noop_buf[CI_MAX_CMD_INGEST] = {25,5,192,30,0,1,0,0};
int TEST_MSG_ID = 123;
int TEST_CC = 0;

int32 CI_InitCustom(void)
{
	int32 Status = -1;

    if(INIT_CUSTOM_RET == NOMINAL)
    {
    	CFE_EVS_SendEvent(CI_ENA_INF_EID, CFE_EVS_INFORMATION,
					  "UDP command input enabled on port");
		Status = 0;
    }	
    else if(INIT_CUSTOM_RET == SOCK_ERR)
    {
    	CFE_EVS_SendEvent(CI_SOCKET_ERR_EID, CFE_EVS_ERROR, "Socket errno");
    }
    else if(INIT_CUSTOM_RET == BIND_ERR)
    {
    	CFE_EVS_SendEvent(CI_SOCKET_ERR_EID, CFE_EVS_ERROR,"Bind socket failed");
    }

	return Status;
}

int32 CI_ReadMessage(char* buffer, uint32* size)
{
	/* Create CFE_SB_Msg_t */
	if(READ_MSG_RET == CI_CMD)
	{
		CI_NoArgCmd_t 	cmd;
		uint32  		MsgSize = sizeof(cmd);
		CFE_SB_InitMsg(&cmd, CI_CMD_MID, MsgSize, TRUE);
		*size = &cmd;
	}
	else if(READ_MSG_RET == EXT_STEP_1)
	{
		CI_CmdRegData_t 	cmd;
		CI_CmdRegData_t 	*regDataPtr;
		uint32  			MsgSize = sizeof(cmd);
		regDataPtr = ((CI_CmdRegData_t *) &cmd);
		regDataPtr->msgID = TEST_MSG_ID;
		regDataPtr->step = STEP_1;
		CI_CmdRegister(&cmd);
		*size = &cmd;
	}
	else if(READ_MSG_RET == EXT_STEP_2)
	{
		CI_CmdRegData_t 	cmd;
		CI_CmdRegData_t 	*regDataPtr;
		uint32  			MsgSize = sizeof(cmd);
		regDataPtr = ((CI_CmdRegData_t *) &cmd);
		regDataPtr->msgID = TEST_MSG_ID;
		regDataPtr->step = STEP_2;
		CI_CmdRegister(&cmd);
		*size = &cmd;
	}

	return 0;
}


int32 CI_CleanupCustom(void)
{
    return 0;
}


