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

#include "cfe.h"
#include "ci_mission_cfg.h"
#include "app.h"
#include "msg.h"
#include "ci_platform_cfg.h"
#include "events.h"
#include "version.h"
#include <termio.h>
#include <stdlib.h>

#ifdef TRICK_VER
#include "sch_proto.h"
#endif

/*
** Local Defines
*/

/*
** Local Structure Declarations
*/

/*
** External Global Variables
*/

CI_AppData_T CI_AppData;

/*
** Local Variables
*/


/*
** Local Function Prototypes
*/
void CI_AppMain(void)
{
	int32     iStatus;

	CFE_ES_PerfLogEntry(CI_MAIN_TASK_PERF_ID);

	/* Initialize the CI app*/
	iStatus = CI_AppInit();

	if (iStatus != CFE_SUCCESS)
	{
		CI_AppData.RunStatus= CFE_ES_APP_ERROR;
	}

	/* Application Main Loop */
	while(CFE_ES_RunLoop(&CI_AppData.RunStatus) == TRUE)
	{
		/* Call RecvMsg to pend on sch wakeup msg */
   		CI_RecvMsg(CI_AppData.Config->Timeout);
	}

	/*  Exit the Application */
	CFE_ES_ExitApp(CI_AppData.RunStatus);
}



int32 CI_RecvMsg( int32 timeout )
{
	int32 iStatus ;

    /*  Performance Log Exit Stamp */
    CFE_ES_PerfLogExit(CI_MAIN_TASK_PERF_ID);

    /* Wait for SCH software bus Wakeup or HK message */
    iStatus = CFE_SB_RcvMsg(&CI_AppData.MsgPtr,
    		                 CI_AppData.SchPipe,
    		                 timeout);
    /* Message received now process message */
    if (iStatus == CFE_SUCCESS)
    {
        CI_ProcessSchMsg();
    }
    else if ( iStatus == CFE_SB_TIME_OUT )
	{
    	/* SCH was probably unloaded or the entry was disabled, causing the
    	 * wakeup to timeout.  Rather than completely blackout all commanding
    	 * we'll go ahead and poll for commands after the timeout anyway.
    	 */
        CI_ProcessCmdMsg();
        CI_OutputStatus();
    }
    else if ( iStatus == CFE_SB_NO_MESSAGE )
    {
        /* nothing to be done if no message is present */
    }

    /* Performance Log Exit Stamp */
    CFE_ES_PerfLogEntry(CI_MAIN_TASK_PERF_ID);

    return iStatus ;

}



void CI_Cleanup(void)
{
	uint32 i = 0;

    /* If sockets connected close the sockets */
	if(CI_AppData.Config != 0)
	{
		for(i=0; i < CI_MAX_SYNC_CHANNELS; i++)
		{
			if(CI_AppData.Config->SyncChannel[i].Mode == CI_CHANNEL_ENABLED)
			{
				close(CI_AppData.Config->SyncChannel[i].Socket);
			}
		}
		CI_AppData.Config = 0;
	}
}



int32 CI_AppInit(void)
{
    int32  iStatus = CFE_SUCCESS;
    uint32 i = 0;

    CI_AppData.RunStatus = CFE_ES_APP_RUN;

    /* Register the CI app */
    CFE_ES_RegisterApp();

    /* Setup Command Ingest data */
	CI_AppData.IngestPointer = (CFE_SB_Msg_t *) &CI_AppData.IngestBuffer[0];

    /*  Setup CMD Pipes */
    CI_AppData.CmdPipeDepth = CI_CMD_PIPE_DEPTH ;
    strcpy(CI_AppData.CmdPipeName,  "CI_CMD_PIPE");

    CI_AppData.SchPipeDepth = CI_SCH_PIPE_DEPTH;
    strcpy(CI_AppData.SchPipeName,  "CI_SCH_PIPE");

    /* Initialize event filter table */
    CI_AppData.EventFilters[0].EventID = CI_INIT_INF_EID;
    CI_AppData.EventFilters[0].Mask    = CFE_EVS_NO_FILTER;
    CI_AppData.EventFilters[1].EventID = CI_CCIDPIPE_ERR_EID;
    CI_AppData.EventFilters[1].Mask    = CFE_EVS_NO_FILTER;
    CI_AppData.EventFilters[2].EventID = CI_SUBSCRIBE_ERR_EID;
    CI_AppData.EventFilters[2].Mask    = CFE_EVS_NO_FILTER;
    CI_AppData.EventFilters[3].EventID = CI_SOCKET_ERR_EID;
    CI_AppData.EventFilters[3].Mask    = CFE_EVS_NO_FILTER;
    CI_AppData.EventFilters[4].EventID = CI_TLMOUTSTOP_ERR_EID;
    CI_AppData.EventFilters[4].Mask    = CFE_EVS_NO_FILTER;
    CI_AppData.EventFilters[5].EventID = CI_MSGID_ERR_EID;
    CI_AppData.EventFilters[5].Mask    = CFE_EVS_NO_FILTER;
    CI_AppData.EventFilters[6].EventID = CI_FNCODE_ERR_EID;
    CI_AppData.EventFilters[6].Mask    = CFE_EVS_EVERY_OTHER_TWO;
    CI_AppData.EventFilters[7].EventID = CI_NOOP_INF_EID;
    CI_AppData.EventFilters[7].Mask    = CFE_EVS_NO_FILTER;

    /* Register event filter table */
    iStatus = CFE_EVS_Register(CI_AppData.EventFilters,
    		                   CI_EVT_COUNT,
    		                   CFE_EVS_BINARY_FILTER);
    if ( iStatus != CFE_SUCCESS )
    {
    	CFE_ES_WriteToSysLog("CI App: Error Registering Events");
    }

    /* Initialize housekeeping packet (clear user data area) */
    CFE_SB_InitMsg(&CI_AppData.HkPacket,
                    CI_HK_TLM_MID,
                    sizeof(CI_AppData.HkPacket), TRUE);

    /* Subscribe for Commands on the Command Pipe */
    iStatus = CFE_SB_CreatePipe(&CI_AppData.CmdPipe,
    		                    CI_AppData.CmdPipeDepth,
    		                    CI_AppData.CmdPipeName);
    if (iStatus == CFE_SUCCESS)
    {
    	CFE_SB_Subscribe(CI_CMD_MID, CI_AppData.CmdPipe);
    }
    else
    {
    	CFE_EVS_SendEvent(CI_CCIDPIPE_ERR_EID,CFE_EVS_ERROR,
    		             "CI: Can't create cmd pipe status %i",iStatus);
    }

    /* Subscribe for the Wakeup and Send_HK messages on the SCH Pipe*/
    iStatus = CFE_SB_CreatePipe(&CI_AppData.SchPipe,
      		                    CI_AppData.SchPipeDepth,
       		                    CI_AppData.SchPipeName);
    if (iStatus == CFE_SUCCESS)
    {
    	CFE_SB_Subscribe(CI_SEND_HK_MID, CI_AppData.SchPipe);
    	CFE_SB_Subscribe(CI_READ_CMD_MID,  CI_AppData.SchPipe);
    }
    else
    {
    	CFE_EVS_SendEvent(CI_CCIDPIPE_ERR_EID,CFE_EVS_ERROR,
    	                 "CI: Can't create cmd pipe status %i",iStatus);
    }

    /* Install the delete handler */
    OS_TaskInstallDeleteHandler((void *)(&CI_Cleanup));

    /* Register and Load iLoad Table */
    iStatus = CI_TableInit();
    if(iStatus != CFE_SUCCESS)
    {
    	goto end_of_function;
    }

    CI_InitIncomingChannels();

    CI_AppData.DropFileData = FALSE;
    CI_AppData.DropEOF = FALSE;
    CI_AppData.DropFIN = FALSE;
	CI_AppData.DropACK = FALSE;
	CI_AppData.DropMetaData = FALSE;
	CI_AppData.DropNAK = FALSE;
	CI_AppData.CorruptChecksum = FALSE;
	CI_AppData.AdjustFileSize = FALSE;
	CI_AppData.HkPacket.SchedMode = CI_SCHED_MODE_SYNCHRONOUS;

    /* Initialize listener child task. */
    iStatus = CI_InitListenerTask();
    if(iStatus != CFE_SUCCESS)
    {
    	goto end_of_function;
    }

    CFE_EVS_SendEvent (CI_INIT_INF_EID, CFE_EVS_INFORMATION,
               "CI Initialized. Version %d.%d.%d.%d.",
                CI_MAJOR_VERSION,
                CI_MINOR_VERSION,
                CI_REVISION,
                CI_MISSION_REV);

end_of_function:
    return iStatus;
}



void CI_ProcessSchMsg(void)
{
	CFE_SB_MsgId_t  MsgId;

	MsgId = CFE_SB_GetMsgId(CI_AppData.MsgPtr);
    switch (MsgId)
    {
    	case CI_READ_CMD_MID:
    	{
    		//CI_ReadSyncUpLink();

            /* Process Incoming Commands */
            //CI_ProcessCmdMsg();

            break;
    	}

        case CI_SEND_HK_MID:
    	{
             CI_OutputStatus();

             break;
    	}

        default:
    	{
             CFE_EVS_SendEvent(CI_MSGID_ERR_EID,CFE_EVS_ERROR,
            		           "CI: Sch Pipe Invalid Msg ID Rcvd 0x%x",MsgId);
             break;
    	}
    }
}



void CI_ProcessPDU(void)
{
	CF_PDU_Hdr_t *PduHdrPtr;
	uint8 *PduDataPtr;
	uint8 *IncomingPduPtr;
	uint8 PduData0;
	uint8 EntityIdBytes, TransSeqBytes, PduHdrBytes;
	CFE_SB_MsgId_t MessageID = CFE_SB_GetMsgId(CI_AppData.IngestPointer);
	boolean sendToSB = FALSE;
	uint32 *checkSumPtr;
	uint32 *fileSizePtr;

	if (MessageID == CI_AppData.Config->PDUMessageID)
	{
		IncomingPduPtr = ((uint8 *)CI_AppData.IngestPointer);
		if (CFE_TST(MessageID, 12) == 1)
		{
			IncomingPduPtr += CFE_SB_CMD_HDR_SIZE;
		}
		else
		{
			IncomingPduPtr += CFE_SB_TLM_HDR_SIZE;
		}

		PduHdrPtr = (CF_PDU_Hdr_t *)IncomingPduPtr;
		/* Swap Length */
		PduHdrPtr->PDataLen = ntohs(PduHdrPtr->PDataLen);

		/* calculate size of incoming pdu to ensure we don't overflow the buf */
		EntityIdBytes = ((PduHdrPtr->Octet4 >> 4) & 0x07) + 1;
		TransSeqBytes = (PduHdrPtr->Octet4 & 0x07) + 1;
		PduHdrBytes = 4 + (EntityIdBytes * 2) + TransSeqBytes;

		CI_AppData.HkPacket.PDUsCaptured++;

		/* Check if a File Data PDU  was rcv'd */
		if (CFE_TST(PduHdrPtr->Octet1,4))
		{
			OS_printf("CI: File Data PDU rcv'd\n");
			if ((CI_AppData.DropFileData == TRUE) && (CI_AppData.DropFileDataCnt > 0))
			{
				CI_AppData.DropFileDataCnt--;
				OS_printf("CI: File Data PDU dropped\n");
				CI_AppData.HkPacket.FDPdusDropped++;
			}
			else
			{
				sendToSB = TRUE;
				CI_AppData.DropFileData = FALSE;
			}
		}
		else
		{
			/* Not a File Data PDU */
			PduDataPtr = (uint8 *)PduHdrPtr + PduHdrBytes;
			PduData0 = *PduDataPtr;

			OS_printf("CI: PDU Data Type = %d\n",PduData0);
			switch (PduData0)
			{
				case 4:
					OS_printf("CI: EOF PDU rcv'd\n");
					if ((CI_AppData.DropEOF == TRUE) && (CI_AppData.DropEOFCnt > 0))
					{
						CI_AppData.DropEOFCnt--;
						OS_printf("CI: EOF PDU dropped\n");
						CI_AppData.HkPacket.EOFPdusDropped++;
					}
					else
					{
						sendToSB = TRUE;
						CI_AppData.DropEOF = FALSE;
					}

					PduDataPtr += 2;
					checkSumPtr = (uint32 *)PduDataPtr;
					fileSizePtr = checkSumPtr + 1;

					if (CI_AppData.CorruptChecksum == TRUE)
					{
						OS_printf("CI: good checksum = %x\n",*checkSumPtr);
						/* Corrupt the checksum */
						*checkSumPtr = 0x12345678;
						OS_printf("CI: corrupted checksum = %x\n",*checkSumPtr);
						CI_AppData.CorruptChecksum = FALSE;
					}

					if (CI_AppData.AdjustFileSize == TRUE)
					{
						OS_printf("CI: good file size = %d\n",*fileSizePtr);
						/* Adjust the file size */
						*fileSizePtr += CI_AppData.PDUFileSizeAdjustment;
						OS_printf("CI: adjusted file size = %d\n",*fileSizePtr);
						CI_AppData.AdjustFileSize = FALSE;
					}

					break;

				case 5:
					OS_printf("CI: FIN PDU rcv'd\n");
					if ((CI_AppData.DropFIN == TRUE) && (CI_AppData.DropFINCnt > 0))
					{
						CI_AppData.DropFINCnt--;
						OS_printf("CI: FIN PDU dropped\n");
						CI_AppData.HkPacket.FINPdusDropped++;
					}
					else
					{
						sendToSB = TRUE;
						CI_AppData.DropFIN = FALSE;
					}

					break;

				case 6:
					OS_printf("CI: ACK PDU rcv'd\n");
					if ((CI_AppData.DropACK == TRUE) && (CI_AppData.DropACKCnt > 0))
					{
						CI_AppData.DropACKCnt--;
						OS_printf("CI: ACK PDU dropped\n");
						CI_AppData.HkPacket.ACKPdusDropped++;
					}
					else
					{
						sendToSB = TRUE;
						CI_AppData.DropACK = FALSE;
					}

					break;

				case 7:
					OS_printf("CI: Meta Data PDU rcv'd\n");
					if ((CI_AppData.DropMetaData == TRUE) && (CI_AppData.DropMetaDataCnt > 0))
					{
						CI_AppData.DropMetaDataCnt--;
						OS_printf("CI: Meta Data PDU dropped\n");
						CI_AppData.HkPacket.MDPdusDropped++;
					}
					else
					{
						sendToSB = TRUE;
						CI_AppData.DropMetaData = FALSE;
					}

					break;

				case 8:
					OS_printf("CI: NAK PDU rcv'd\n");
					if ((CI_AppData.DropNAK == TRUE) && (CI_AppData.DropNAKCnt > 0))
					{
						CI_AppData.DropNAKCnt--;
						OS_printf("CI: NAK PDU dropped\n");
						CI_AppData.HkPacket.NAKPdusDropped++;
					}
					else
					{
						sendToSB = TRUE;
						CI_AppData.DropNAK = FALSE;
					}

					break;

				default:
					break;
			}
		}
	}
	else
	{
		sendToSB = TRUE;
	}

	if (sendToSB == TRUE)
	{
		CFE_ES_PerfLogEntry(CI_SOCKET_RCV_PERF_ID);
		CI_AppData.HkPacket.IngestMsgCount++;
		CFE_SB_SendMsg(CI_AppData.IngestPointer);
		CFE_ES_PerfLogExit(CI_SOCKET_RCV_PERF_ID);
	}

	return;

} /* End of CI_ProcessPDU() */



void CI_ReadSyncUpLink(void)
{
	uint32 i = 0;
    int MsgSize = 0;
    uint32 iMsg = 0;

    /* For loop to call receive a max of ten times */
	for(i=0; i < CI_MAX_SYNC_CHANNELS; i++)
    {
    	/* Receive data and place in IngestBuffer */
	    for (iMsg = 0; iMsg <= 10; iMsg++)
		{
			if(CI_AppData.Config->SyncChannel[i].Mode == CI_CHANNEL_ENABLED)
			{
				CFE_ES_PerfLogEntry(CI_SOCKET_RCV_PERF_ID);
				MsgSize = recv(CI_AppData.Config->SyncChannel[i].Socket,
								   (char *)&CI_AppData.IngestBuffer,
								   sizeof(CI_AppData.IngestBuffer), MSG_DONTWAIT);
				while(MsgSize > 0) {
					/* Check to see if data was receieved) */
					if ( (MsgSize < 0) && (errno == EWOULDBLOCK) )
					{
						break; /* no (more) messages */
					}
					else if( MsgSize < 0)
					{
						CI_AppData.HkPacket.IngestErrorCount++;
						CFE_EVS_SendEvent(CI_CMD_INGEST_ERR_EID, CFE_EVS_ERROR,
										  "CI: recv() returned error.  errno = %i\n",
										  errno );
					}
					else
					{
						/* If number of bytes received less than max */
						if (MsgSize <= CI_MAX_CMD_INGEST)
						{
							CFE_SB_MsgId_t  msgID = 0;
							uint16 cmdCode = 0;

							msgID = CFE_SB_GetMsgId((CFE_SB_MsgPtr_t)CI_AppData.IngestBuffer);
							cmdCode = CFE_SB_GetCmdCode((CFE_SB_MsgPtr_t)CI_AppData.IngestBuffer);

							CI_ProcessPDU();
						}
						else
						{
							CI_AppData.HkPacket.IngestErrorCount++;
							CFE_EVS_SendEvent(CI_CMD_INGEST_ERR_EID, CFE_EVS_ERROR,
											  "CI: L%d, cmd %0x %0x dropped, too long\n",
											  __LINE__, *(long *)CI_AppData.IngestBuffer,
											  *(long *)(CI_AppData.IngestBuffer + 4) );
						}
					}

					MsgSize = recv(CI_AppData.Config->SyncChannel[i].Socket,
									   (char *)&CI_AppData.IngestBuffer,
									   sizeof(CI_AppData.IngestBuffer), MSG_DONTWAIT);
				}

				CFE_ES_PerfLogExit(CI_SOCKET_RCV_PERF_ID);
			}
		}
    }
}



void CI_ProcessCmdMsg(void)
{
    CFE_SB_Msg_t    *MsgPtr;
    CFE_SB_MsgId_t  MsgId;

    while(1)    /* While loop to execute until all commands received */
    {
       /* Check for message on cmd pipe and process */
       switch (CFE_SB_RcvMsg(&MsgPtr, CI_AppData.CmdPipe, CFE_SB_POLL))
       {
          case CFE_SUCCESS:

               MsgId = CFE_SB_GetMsgId(MsgPtr);

               switch (MsgId)
               {
                  case CI_CMD_MID:
                	   /* Call Command Exec Function */
                       CI_CmdExec(MsgPtr);
                       break;

                  default:
                       CFE_EVS_SendEvent(CI_MSGID_ERR_EID,CFE_EVS_ERROR,
                    		         "CI: Cmd Pipe Invalid Msg ID Rcvd 0x%x",
                    		         MsgId);
                       break;
               }
               break;
            default:
               /* Exit command processing loop if no message received. */
               return;
       }
    }
}



void CI_OutputStatus(void)
{
	/* Timestamp Message */
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) &CI_AppData.HkPacket);

    CFE_SB_SendMsg((CFE_SB_Msg_t *) &CI_AppData.HkPacket);
}



int32 CI_TableInit (void)
{
	int32 iStatus = CFE_SUCCESS;

    /* Register The CI config table */
    iStatus = CFE_TBL_Register (&CI_AppData.ConfigHandle,
    		CI_CONFIG_TABLENAME, (sizeof (CI_ConfigTable_t)),
    		CFE_TBL_OPT_DEFAULT,
    		(CFE_TBL_CallbackFuncPtr_t)CI_ValidateTable);

    if (iStatus != CFE_SUCCESS)
    {
    	CFE_EVS_SendEvent(CI_CONFIG_REG_ERR_EID, CFE_EVS_ERROR,
    			"CFE_TBL_Register() returned error %i.  Aborting table init.",
    			iStatus);

        goto end_of_function;
    }

    /* Load the CI configuration table file */
    iStatus = CFE_TBL_Load (CI_AppData.ConfigHandle, CFE_TBL_SRC_FILE,
    		CI_CONFIG_FILENAME);

    if (iStatus != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(CI_CONFIG_LD_ERR_EID, CFE_EVS_ERROR,
                  "CFE_TBL_Load() returned error %i.  Aborting table init.",
                  iStatus);

        goto end_of_function;
    }

    /* Manage the CI config table */
    iStatus = CFE_TBL_Manage(CI_AppData.ConfigHandle);
    if (iStatus != CFE_SUCCESS)
    {
    	CFE_EVS_SendEvent(CI_CONFIG_MNG_ERR_EID, CFE_EVS_ERROR,
           "CFE_TBL_Manage() returned error %i.  Aborting table init.",
           iStatus);

        goto end_of_function;
    }

    /* Make sure the CI iLoad Table is accessible */
    iStatus = CFE_TBL_GetAddress ((void **) (& CI_AppData.Config),
    		                                CI_AppData.ConfigHandle);
    /* Status should be CFE_TBL_INFO_UPDATED because we loaded it above */
    if (iStatus != CFE_TBL_INFO_UPDATED)
    {
    	CFE_EVS_SendEvent(CI_CONFIG_GADR_ERR_EID, CFE_EVS_ERROR,
    			"CFE_TBL_GetAddress() returned error %i.  Aborting table init.",
    			iStatus);

    	goto end_of_function;
    }

    iStatus = CFE_SUCCESS;

end_of_function:
    return iStatus;

}



int32 CI_ValidateTable (CI_ConfigTable_t* table)
{
	return 0;
}



int32 CI_InitListenerTask(void)
{
	int32 Status = CFE_SUCCESS;

    Status= CFE_ES_CreateChildTask(&CI_AppData.ListenerTaskID,
                                   CI_LISTENER_TASK_NAME,
								   CI_ListenerTaskMain,
                                   NULL,
                                   CI_LISTENER_TASK_STACK_SIZE,
                                   CI_LISTENER_TASK_PRIORITY,
                                   0);
    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent (CI_LISTENER_CREATE_CHDTASK_ERR_EID,
                           CFE_EVS_ERROR,
                           "Listener child task failed.  CFE_ES_CreateChildTask returned: 0x%08X",
                           Status);
        goto end_of_function;
    }

end_of_function:
    return Status;

}



void CI_ListenerTaskMain(void)
{
	uint32 i = 0;
    int MsgSize = 0;
    uint32 iMsg = 0;

    CFE_ES_RegisterApp();

    /* Receive data and place in IngestBuffer */
    while(1)
    {
		MsgSize = recv(CI_AppData.Config->AsyncChannel.Socket,
						   (char *)&CI_AppData.IngestBuffer,
						   sizeof(CI_AppData.IngestBuffer), 0);
		if(MsgSize > 0) {
			/* If number of bytes received less than max */
			if (MsgSize <= CI_MAX_CMD_INGEST)
			{
				CFE_ES_PerfLogEntry(CI_SOCKET_RCV_PERF_ID);
				CFE_SB_MsgId_t  msgID = 0;
				uint16 cmdCode = 0;

				msgID = CFE_SB_GetMsgId((CFE_SB_MsgPtr_t)CI_AppData.IngestBuffer);
				cmdCode = CFE_SB_GetCmdCode((CFE_SB_MsgPtr_t)CI_AppData.IngestBuffer);

				CI_AppData.HkPacket.IngestMsgCount++;
				CFE_SB_SendMsg(CI_AppData.IngestBuffer);
				CFE_ES_PerfLogExit(CI_SOCKET_RCV_PERF_ID);
			}
			else
			{
				CI_AppData.HkPacket.IngestErrorCount++;
				CFE_EVS_SendEvent(CI_CMD_INGEST_ERR_EID, CFE_EVS_ERROR,
								  "CI: L%d, cmd %0x %0x dropped, too long\n",
								  __LINE__, *(long *)CI_AppData.IngestBuffer,
								  *(long *)(CI_AppData.IngestBuffer + 4) );
			}
		}
    }
}



int32 CI_InitIncomingChannels(void)
{
	uint32 i = 0;
	struct sockaddr_in address;
	int optval = 1;
	int32 Status = CFE_SUCCESS;

	if((CI_AppData.Config->AsyncChannel.Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		CFE_EVS_SendEvent(CI_SOCKET_ERR_EID, CFE_EVS_ERROR,
			   "CI: socket errno: %i on channel %u", errno, i);
		Status = -1;
		goto end_of_function;
	}

	setsockopt(CI_AppData.Config->AsyncChannel.Socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	bzero((char *) &address, sizeof(address));
	address.sin_family      = AF_INET;
	address.sin_addr.s_addr = htonl (INADDR_ANY);
	address.sin_port        = htons(CI_AppData.Config->AsyncChannel.Port);

	if ( (bind(CI_AppData.Config->AsyncChannel.Socket, (struct sockaddr *) &address, sizeof(address)) < 0) )
	{
		CFE_EVS_SendEvent(CI_SOCKET_ERR_EID, CFE_EVS_ERROR,"CI: bind socket failed = %d", errno);
		Status = -1;
		goto end_of_function;
	}

	CFE_EVS_SendEvent(CI_ENA_INF_EID, CFE_EVS_INFORMATION,
					  "CI UDP command input enabled channel %u on port %u.",
					  i, CI_AppData.Config->AsyncChannel.Port);

    for (i=0; i < CI_MAX_SYNC_CHANNELS; i++)
    {
    	if(CI_AppData.Config->SyncChannel[i].Mode == CI_CHANNEL_ENABLED)
    	{
			struct sockaddr_in address;
			int optval = 1;

			if((CI_AppData.Config->SyncChannel[i].Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
			{
				CI_AppData.Config->SyncChannel[i].Mode = CI_CHANNEL_DISABLED;
				CFE_EVS_SendEvent(CI_SOCKET_ERR_EID, CFE_EVS_ERROR,
					   "CI: socket errno: %i on channel %u", errno, i);
				continue;
			}

			setsockopt(CI_AppData.Config->SyncChannel[i].Socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

			bzero((char *) &address, sizeof(address));
			address.sin_family      = AF_INET;
			address.sin_addr.s_addr = htonl (INADDR_ANY);
			address.sin_port        = htons(CI_AppData.Config->SyncChannel[i].Port);

			if ( (bind(CI_AppData.Config->SyncChannel[i].Socket, (struct sockaddr *) &address, sizeof(address)) < 0) )
			{
				CFE_EVS_SendEvent(CI_SOCKET_ERR_EID, CFE_EVS_ERROR,"CI: bind socket failed = %d", errno);
				continue;
			}

			CFE_EVS_SendEvent(CI_ENA_INF_EID, CFE_EVS_INFORMATION,
							  "CI UDP command input enabled channel %u on port %u.",
							  i, CI_AppData.Config->SyncChannel[i].Port);
    	}
    }

end_of_function:
    return Status;
}

#ifdef __cplusplus
}
#endif
