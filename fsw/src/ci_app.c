
/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include <string.h>

#include "cfe.h"

#include "ci_app.h"
#include "ci_msg.h"
#include "ci_version.h"

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/
CI_AppData_t  CI_AppData;

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Definitions
*************************************************************************/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize event tables                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 CI_InitEvent()
{
    int32  iStatus=CFE_SUCCESS;
    int32  ind = 0;

    /* Initialize the event filter table.
     * Note: 0 is the CFE_EVS_NO_FILTER mask and event 0 is reserved (not used) */
    memset((void*)CI_AppData.EventTbl, 0x00, sizeof(CI_AppData.EventTbl));

    /* TODO: Choose the events you want to filter.  CFE_EVS_MAX_EVENT_FILTERS
     * limits the number of filters per app.  An explicit CFE_EVS_NO_FILTER 
     * (the default) has been provided as an example. */
    CI_AppData.EventTbl[  ind].EventID = CI_RESERVED_EID;
    CI_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    CI_AppData.EventTbl[  ind].EventID = CI_INF_EID;
    CI_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    CI_AppData.EventTbl[  ind].EventID = CI_CONFIG_TABLE_ERR_EID;
    CI_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    CI_AppData.EventTbl[  ind].EventID = CI_CDS_ERR_EID;
    CI_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    CI_AppData.EventTbl[  ind].EventID = CI_PIPE_ERR_EID;
    CI_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    CI_AppData.EventTbl[  ind].EventID = CI_MSGID_ERR_EID;
    CI_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    CI_AppData.EventTbl[  ind].EventID = CI_MSGLEN_ERR_EID;
    CI_AppData.EventTbl[ind++].Mask    = CFE_EVS_NO_FILTER;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(CI_AppData.EventTbl,
                               CI_EVT_CNT, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("CI - Failed to register with EVS (0x%08X)\n", (unsigned int)iStatus);
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Message Pipes                                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 CI_InitPipe()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init schedule pipe and subscribe to wakeup messages */
    iStatus = CFE_SB_CreatePipe(&CI_AppData.SchPipeId,
                                 CI_SCH_PIPE_DEPTH,
                                 CI_SCH_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(CI_WAKEUP_MID, CI_AppData.SchPipeId, CFE_SB_Default_Qos, CI_SCH_PIPE_WAKEUP_RESERVED);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(CI_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "Sch Pipe failed to subscribe to CI_WAKEUP_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto CI_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_SubscribeEx(CI_SEND_HK_MID, CI_AppData.SchPipeId, CFE_SB_Default_Qos, CI_SCH_PIPE_SEND_HK_RESERVED);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(CI_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "CMD Pipe failed to subscribe to CI_SEND_HK_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto CI_InitPipe_Exit_Tag;
        }

    }
    else
    {
        (void) CFE_EVS_SendEvent(CI_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create SCH pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto CI_InitPipe_Exit_Tag;
    }

    /* Init command pipe and subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&CI_AppData.CmdPipeId,
                                 CI_CMD_PIPE_DEPTH,
                                 CI_CMD_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(CI_CMD_MID, CI_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(CI_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "CMD Pipe failed to subscribe to CI_CMD_MID. (0x%08X)",
                                     (unsigned int)iStatus);
            goto CI_InitPipe_Exit_Tag;
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(CI_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create CMD pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto CI_InitPipe_Exit_Tag;
    }

    /* Init data pipe and subscribe to messages on the data pipe */
    iStatus = CFE_SB_CreatePipe(&CI_AppData.DataPipeId,
                                 CI_DATA_PIPE_DEPTH,
                                 CI_DATA_PIPE_NAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* TODO:  Add CFE_SB_Subscribe() calls for other apps' output data here.
        **
        ** Examples:
        **     CFE_SB_Subscribe(GNCEXEC_OUT_DATA_MID, CI_AppData.DataPipeId);
        */
    }
    else
    {
        (void) CFE_EVS_SendEvent(CI_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create Data pipe (0x%08X)",
                                 (unsigned int)iStatus);
        goto CI_InitPipe_Exit_Tag;
    }

CI_InitPipe_Exit_Tag:
    return (iStatus);
}
    

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize Global Variables                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 CI_InitData()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init input data */
    memset((void*)&CI_AppData.InData, 0x00, sizeof(CI_AppData.InData));

    /* Init output data */
    memset((void*)&CI_AppData.OutData, 0x00, sizeof(CI_AppData.OutData));
    CFE_SB_InitMsg(&CI_AppData.OutData,
                   CI_OUT_DATA_MID, sizeof(CI_AppData.OutData), TRUE);

    /* Init housekeeping packet */
    memset((void*)&CI_AppData.HkTlm, 0x00, sizeof(CI_AppData.HkTlm));
    CFE_SB_InitMsg(&CI_AppData.HkTlm,
                   CI_HK_TLM_MID, sizeof(CI_AppData.HkTlm), TRUE);

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* CI initialization                                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 CI_InitApp()
{
    int32  iStatus   = CFE_SUCCESS;
    int8   hasEvents = 0;

    iStatus = CI_InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("CI - Failed to init events (0x%08X)\n", (unsigned int)iStatus);
        goto CI_InitApp_Exit_Tag;
    }
    else
    {
        hasEvents = 1;
    }

    iStatus = CI_InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(CI_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init pipes (0x%08X)",
                                 (unsigned int)iStatus);
        goto CI_InitApp_Exit_Tag;
    }

    iStatus = CI_InitData();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(CI_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init data (0x%08X)",
                                 (unsigned int)iStatus);
        goto CI_InitApp_Exit_Tag;
    }

    iStatus = CI_InitConfigTbl();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(CI_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init config tables (0x%08X)",
                                 (unsigned int)iStatus);
        goto CI_InitApp_Exit_Tag;
    }

    iStatus = CI_InitCdsTbl();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(CI_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init CDS table (0x%08X)",
                                 (unsigned int)iStatus);
        goto CI_InitApp_Exit_Tag;
    }

    /* Initialize custom code. */
    iStatus = CI_InitCustom();
    if(iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("CI - InitCustom failed (%i)\n", (int)iStatus);
        goto CI_InitApp_Exit_Tag;
    }

    /* Register the cleanup callback */
    iStatus = OS_TaskInstallDeleteHandler(&CI_CleanupCallback);
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(CI_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to init register cleanup callback (0x%08X)",
                                 (unsigned int)iStatus);
        goto CI_InitApp_Exit_Tag;
    }

    /* Initialize listener child task. */
    iStatus = CI_InitListenerTask();
    if(iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("CI - Failed to initialize listener child task (0x%08X)\n", (unsigned int)iStatus);
        goto CI_InitApp_Exit_Tag;
    }

    /* Register the cleanup callback */
    OS_TaskInstallDeleteHandler((void*)&CI_CleanupCallback);

CI_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        (void) CFE_EVS_SendEvent(CI_INIT_INF_EID, CFE_EVS_INFORMATION,
                                 "Initialized.  Version %d.%d.%d.%d",
                                 CI_MAJOR_VERSION,
                                 CI_MINOR_VERSION,
                                 CI_REVISION,
                                 CI_MISSION_REV);
    }
    else
    {
        if (hasEvents == 1)
        {
            (void) CFE_EVS_SendEvent(CI_INIT_ERR_EID, CFE_EVS_ERROR, "Application failed to initialize");
        }
        else
        {
            (void) CFE_ES_WriteToSysLog("CI - Application failed to initialize\n");
        }
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* CI Cleanup                                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CI_CleanupCallback()
{
    CI_CleanupCustom();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */




/* TODO:  Add Doxygen markup. */
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
    }

    return Status;

}


/* TODO:  Add Doxygen markup. */
void CI_ListenerTaskMain(void)
{
    uint32  i       = 0;
    uint32  MsgSize = 0;
    uint32  iMsg    = 0;

    CFE_ES_RegisterChildTask();

    /* Receive data and place in IngestBuffer */
    /* TODO:  Replace infinite loop with something that will terminate on unload. */
    while(1)
    {
        MsgSize = CI_MAX_CMD_INGEST;
        CI_ReadMessage(CI_AppData.IngestBuffer, &MsgSize);
        if(MsgSize > 0) {
            /* If number of bytes received less than max */
            if (MsgSize <= CI_MAX_CMD_INGEST)
            {
                CFE_ES_PerfLogEntry(CI_SOCKET_RCV_PERF_ID);
                CFE_SB_MsgId_t  msgID = 0;
                uint16 cmdCode = 0;

                msgID = CFE_SB_GetMsgId((CFE_SB_MsgPtr_t)CI_AppData.IngestBuffer);
                cmdCode = CFE_SB_GetCmdCode((CFE_SB_MsgPtr_t)CI_AppData.IngestBuffer);

                CI_AppData.HkTlm.IngestMsgCount++;
                CFE_SB_SendMsg(CI_AppData.IngestBuffer);
                CFE_ES_PerfLogExit(CI_SOCKET_RCV_PERF_ID);
            }
            else
            {
                CI_AppData.HkTlm.IngestErrorCount++;
                CFE_EVS_SendEvent(CI_CMD_INGEST_ERR_EID, CFE_EVS_ERROR,
                                  "L%d, cmd %0x %0x dropped, too long",
                                  __LINE__, *(long *)CI_AppData.IngestBuffer,
                                  *(long *)(CI_AppData.IngestBuffer + 4) );
            }
        }
        OS_TaskDelay(100);
    }
}
    

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Receive and Process Messages                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 CI_RcvMsg(int32 iBlocking)
{
    int32           iStatus=CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(CI_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, CI_AppData.SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(CI_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
	{
            case CI_WAKEUP_MID:
                CI_ProcessNewCmds();
                CI_ProcessNewData();

                /* TODO:  Add more code here to handle other things when app wakes up */

                /* The last thing to do at the end of this Wakeup cycle should be to
                 * automatically publish new output. */
                CI_SendOutData();
                break;

            case CI_SEND_HK_MID:
                CI_ReportHousekeeping();
                break;

            default:
                (void) CFE_EVS_SendEvent(CI_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid SCH msgId (0x%04X)", (unsigned short)MsgId);
        }
    }
    else if (iStatus == CFE_SB_NO_MESSAGE)
    {
        /* TODO: If there's no incoming message, you can do something here, or 
         * nothing.  Note, this section is dead code only if the iBlocking arg
         * is CFE_SB_PEND_FOREVER. */
        iStatus = CFE_SUCCESS;
    }
    else if (iStatus == CFE_SB_TIME_OUT)
    {
        /* TODO: If there's no incoming message within a specified time (via the
         * iBlocking arg, you can do something here, or nothing.  
         * Note, this section is dead code only if the iBlocking arg
         * is CFE_SB_PEND_FOREVER. */
        iStatus = CFE_SUCCESS;
    }
    else
    {
        /* TODO: This is an example of exiting on an error (either CFE_SB_BAD_ARGUMENT, or
         * CFE_SB_PIPE_RD_ERROR).
         */
        (void) CFE_EVS_SendEvent(CI_PIPE_ERR_EID, CFE_EVS_ERROR,
			  "SB pipe read error (0x%08X), app will exit", (unsigned int)iStatus);
        CI_AppData.uiRunStatus= CFE_ES_APP_ERROR;
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Data                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CI_ProcessNewData()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   DataMsgPtr=NULL;
    CFE_SB_MsgId_t  DataMsgId;

    /* Process telemetry messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&DataMsgPtr, CI_AppData.DataPipeId, CFE_SB_POLL);
        if (iStatus == CFE_SUCCESS)
        {
            DataMsgId = CFE_SB_GetMsgId(DataMsgPtr);
            switch (DataMsgId)
            {
                /* TODO:  Add code to process all subscribed data here
                **
                ** Example:
                **     case NAV_OUT_DATA_MID:
                **         CI_ProcessNavData(DataMsgPtr);
                **         break;
                */

                default:
                    (void) CFE_EVS_SendEvent(CI_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "Recvd invalid data msgId (0x%04X)", (unsigned short)DataMsgId);
                    break;
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            (void) CFE_EVS_SendEvent(CI_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "Data pipe read error (0x%08X)", (unsigned int)iStatus);
            CI_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process Incoming Commands                                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CI_ProcessNewCmds()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   CmdMsgPtr=NULL;
    CFE_SB_MsgId_t  CmdMsgId;

    /* Process command messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, CI_AppData.CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case CI_CMD_MID:
                    CI_ProcessNewAppCmds(CmdMsgPtr);
                    break;

                /* TODO:  Add code to process other subscribed commands here
                **
                ** Example:
                **     case CFE_TIME_DATA_CMD_MID:
                **         CI_ProcessTimeDataCmd(CmdMsgPtr);
                **         break;
                */

                default:
                    /* Bump the command error counter for an unknown command.
                     * (This should only occur if it was subscribed to with this
                     *  pipe, but not handled in this switch-case.) */
                    CI_AppData.HkTlm.usCmdErrCnt++;
                    (void) CFE_EVS_SendEvent(CI_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "Recvd invalid CMD msgId (0x%04X)", (unsigned short)CmdMsgId);
                    break;
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            (void) CFE_EVS_SendEvent(CI_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "CMD pipe read error (0x%08X)", (unsigned int)iStatus);
            CI_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Process CI Commands                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CI_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case CI_NOOP_CC:
                CI_AppData.HkTlm.usCmdCnt++;
                (void) CFE_EVS_SendEvent(CI_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "Recvd NOOP cmd (%u), Version %d.%d.%d.%d",
                                  (unsigned int)uiCmdCode,
                                  CI_MAJOR_VERSION,
                                  CI_MINOR_VERSION,
                                  CI_REVISION,
                                  CI_MISSION_REV);
                break;

            case CI_RESET_CC:
                CI_AppData.HkTlm.usCmdCnt = 0;
                CI_AppData.HkTlm.usCmdErrCnt = 0;
                (void) CFE_EVS_SendEvent(CI_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "Recvd RESET cmd (%u)", (unsigned int)uiCmdCode);
                break;

            /* TODO:  Add code to process the rest of the CI commands here */

            default:
                CI_AppData.HkTlm.usCmdErrCnt++;
                (void) CFE_EVS_SendEvent(CI_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid cmdId (%u)", (unsigned int)uiCmdCode);
                break;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Send CI Housekeeping                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CI_ReportHousekeeping()
{
    /* TODO:  Add code to update housekeeping data, if needed, here.  */

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&CI_AppData.HkTlm);
    int32 iStatus = CFE_SB_SendMsg((CFE_SB_Msg_t*)&CI_AppData.HkTlm);
    if (iStatus != CFE_SUCCESS)
    {
        /* TODO: Decide what to do if the send message fails. */
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Publish Output Data                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CI_SendOutData()
{
    /* TODO:  Add code to update output data, if needed, here.  */

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&CI_AppData.OutData);
    int32 iStatus = CFE_SB_SendMsg((CFE_SB_Msg_t*)&CI_AppData.OutData);
    if (iStatus != CFE_SUCCESS)
    {
        /* TODO: Decide what to do if the send message fails. */
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify Command Length                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

boolean CI_VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
                           uint16 usExpectedLen)
{
    boolean bResult  = TRUE;
    uint16  usMsgLen = 0;

    if (MsgPtr != NULL)
    {
        usMsgLen = CFE_SB_GetTotalMsgLength(MsgPtr);

        if (usExpectedLen != usMsgLen)
        {
            bResult = FALSE;
            CFE_SB_MsgId_t MsgId = CFE_SB_GetMsgId(MsgPtr);
            uint16 usCmdCode = CFE_SB_GetCmdCode(MsgPtr);

            (void) CFE_EVS_SendEvent(CI_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                              "Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                              "msgLen=%d, expectedLen=%d",
                              MsgId, usCmdCode, usMsgLen, usExpectedLen);
            CI_AppData.HkTlm.usCmdErrCnt++;
        }
    }

    return (bResult);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* CI application entry point and main process loop               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CI_AppMain()
{
    /* Register the application with Executive Services */
    CI_AppData.uiRunStatus = CFE_ES_APP_RUN;

    int32 iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        (void) CFE_ES_WriteToSysLog("CI - Failed to register the app (0x%08X)\n", (unsigned int)iStatus);
    }

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(CI_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CI_InitApp();
    }

    if (iStatus == CFE_SUCCESS)
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(CI_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(CI_STARTUP_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(CI_MAIN_TASK_PERF_ID);
    }
    else
    {
        CI_AppData.uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&CI_AppData.uiRunStatus) == TRUE)
    {
        int32 iStatus = CI_RcvMsg(CI_SCH_PIPE_PEND_TIME);
        if (iStatus != CFE_SUCCESS)
        {
            /* TODO: Decide what to do for other return values in CI_RcvMsg(). */
        }
        /* TODO: This is only a suggestion for when to update and save CDS table.
        ** Depends on the nature of the application, the frequency of update
        ** and save can be more or less independently.
        */
        CI_UpdateCdsTbl();
        CI_SaveCdsTbl();

        iStatus = CI_AcquireConfigPointers();
        if(iStatus != CFE_SUCCESS)
        {
            /* We apparently tried to load a new table but failed.  Terminate the application. */
            CI_AppData.uiRunStatus = CFE_ES_APP_ERROR;
        }
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(CI_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(CI_AppData.uiRunStatus);
}

/************************/
/*  End of File Comment */
/************************/
