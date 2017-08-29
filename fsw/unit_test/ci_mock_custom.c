#include "ci_mock_custom.h"
#include "ci_test_utils.h"

int INIT_CUSTOM_RET = NOMINAL;
int READ_MSG_RET;

uint8 ci_noop_buf[CI_MAX_CMD_INGEST] = {25,5,192,0,0,1,0,0};
uint8 ext_buf[CI_MAX_CMD_INGEST] = {28,41,192,0,0,1,0,0};//EA {16,101,192,2,0,1,0,0};
int TEST_MSG_ID = 1065;
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
		// Noop
		CI_NoArgCmd_t 	cmd;
		uint32  		MsgSize = sizeof(cmd);
		CFE_SB_InitMsg(&cmd, CI_CMD_MID, MsgSize, TRUE);
		*size = MsgSize;

		for (int i = 0; i < MsgSize; i++)
		{
			CI_AppData.IngestBuffer[i] = ci_noop_buf[i];
		}
	}
	else if(READ_MSG_RET == EXT_STEP_1)
	{
		CI_CmdRegData_t 	cmd;
		CI_CmdRegData_t 	*regDataPtr;
		uint32  			MsgSize = sizeof(cmd);
		CFE_SB_InitMsg(&cmd, TEST_MSG_ID, MsgSize, TRUE);
		regDataPtr = ((CI_CmdRegData_t *) &cmd);
		regDataPtr->msgID = TEST_MSG_ID;
		regDataPtr->step = STEP_1;
		CI_CmdRegister(&cmd);

		MsgSize = sizeof(CI_NoArgCmd_t);
		*size = MsgSize;
		for (int i = 0; i < MsgSize; i++)
		{
			CI_AppData.IngestBuffer[i] = ext_buf[i];
		}
	}
	else if(READ_MSG_RET == EXT_STEP_2)
	{
		CI_CmdRegData_t 	cmd;
		CI_CmdRegData_t 	*regDataPtr;
		CI_CmdData_t		*CmdData = NULL;
		uint32  			MsgSize = sizeof(cmd);
		CFE_SB_InitMsg(&cmd, TEST_MSG_ID, MsgSize, TRUE);
		regDataPtr = ((CI_CmdRegData_t *) &cmd);
		regDataPtr->msgID = TEST_MSG_ID;
		regDataPtr->step = STEP_2;
		CI_CmdRegister(&cmd);

		MsgSize = sizeof(CI_NoArgCmd_t);
		*size = MsgSize;
		for (int i = 0; i < MsgSize; i++)
		{
			CI_AppData.IngestBuffer[i] = ext_buf[i];
		}
	}
	else if(READ_MSG_RET == EXT_STEP_2_AUTH)
	{
		CI_CmdRegData_t 	cmd;
		CI_CmdRegData_t 	*regDataPtr;
		CI_CmdData_t		*CmdData = NULL;
		uint32  			MsgSize = sizeof(cmd);
		CFE_SB_InitMsg(&cmd, TEST_MSG_ID, MsgSize, TRUE);
		regDataPtr = ((CI_CmdRegData_t *) &cmd);
		regDataPtr->msgID = TEST_MSG_ID;
		regDataPtr->step = STEP_2;
		CI_CmdRegister(&cmd);
		CmdData = CI_GetRegisterdCmd(TEST_MSG_ID, TEST_CC);
		CmdData->state = AUTHORIZED;

		MsgSize = sizeof(CI_NoArgCmd_t);
		*size = MsgSize;
		for (int i = 0; i < MsgSize; i++)
		{
			CI_AppData.IngestBuffer[i] = ext_buf[i];
		}
	}
	else if(READ_MSG_RET == LONG_CMD)
	{
		*size = CI_MAX_CMD_INGEST + 1;
	}

	else if(READ_MSG_RET == INV_CMD)
	{
		CI_NoArgCmd_t 	cmd;
		uint32  		MsgSize = sizeof(cmd);
		CFE_SB_MsgPtr_t CmdMsgPtr;
		CFE_SB_InitMsg(&cmd, CI_CMD_MID, MsgSize, TRUE);
		CmdMsgPtr = (CFE_SB_MsgPtr_t)&cmd;
		CCSDS_WR_VERS(CmdMsgPtr->Hdr, 1);
		*size = MsgSize;
		buffer = &cmd;
	}

	return 0;
}


int32 CI_CleanupCustom(void)
{
    return 0;
}


