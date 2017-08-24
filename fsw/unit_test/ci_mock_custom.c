#include "ci_mock_custom.h"

int INIT_CUSTOM_RET;
int READ_MSG_RET;

//uint8 step_1_buf[CI_MAX_CMD_INGEST] = {1C,29,C0,1C,00,01,00,00};
//uint8 step_2_buf[CI_MAX_CMD_INGEST] = {1C,29,C0,1C,00,01,00,01};
// ea noop 1C 29 C0 1E 00 01 00 00
// ci nnop 19 05 C0 20 00 01 00 00
uint8 ci_noop_buf[CI_MAX_CMD_INGEST] = {25,5,192,30,0,1,0,0};
CFE_SB_MsgPtr_t TestMsgPtr;


int32 CI_InitCustom(void)
{
	int32 Status = -1;
	//printf("Init Ret: %i\n", INIT_CUSTOM_RET);
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
	//printf("Read Ret: %i\n", READ_MSG_RET);
	//uint8 test_buffer[CI_MAX_CMD_INGEST];
    //memset(test_buffer, '\0', CI_MAX_CMD_INGEST);

	if(READ_MSG_RET == CI_CMD)
	{
		//*size = sizeof(CI_NoArgCmd_t);
		//buffer =
	}
	else if(READ_MSG_RET == EXT_STEP_1)
	{
		//*size = sizeof(CI_NoArgCmd_t);
		//*buffer =
	}
	else if(READ_MSG_RET == EXT_STEP_2)
	{
		//*size = sizeof(CI_NoArgCmd_t);
		//*buffer =
	}

	for(int i = 0; i < sizeof(buffer); ++i)
	{

	}
}


int32 CI_CleanupCustom(void)
{
    return 0;
}


