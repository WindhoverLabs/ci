    
#ifndef CI_APP_H
#define CI_APP_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"

#include "ci_platform_cfg.h"
#include "ci_mission_cfg.h"
#include "ci_private_ids.h"
#include "ci_private_types.h"
#include "ci_perfids.h"
#include "ci_msgids.h"
#include "ci_msg.h"
#include "ci_events.h"
#include "ci_config_utils.h"
#include "ci_cds_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Local Defines
*************************************************************************/
#define CI_TIMEOUT_MSEC             	(1000)
#define CI_MAX_CMD_INGEST           	(CFE_SB_MAX_SB_MSG_SIZE)
#define CI_LISTENER_TASK_NAME  		"CI_LISTENER"
#define CI_LISTENER_TASK_STACK_SIZE	16000
#define CI_LISTENER_TASK_PRIORITY	100

/************************************************************************
** Local Structure Definitions
*************************************************************************/

/**
**  \brief CI Operational Data Structure
*/
typedef enum
{
	BHV_OPTIMISTIC,
	BHV_PESSIMISTIC,
} CI_BEHAVIOR;

/**
**  \brief CI Operational Data Structure
*/
typedef struct
{
    /** \brief CFE Event Table */
    CFE_EVS_BinFilter_t  EventTbl[CI_EVT_CNT];

    /**\brief Scheduling Pipe ID */
    CFE_SB_PipeId_t  SchPipeId;

    /** \brief Command Pipe ID */
    CFE_SB_PipeId_t  CmdPipeId;

    /** \brief Data Pipe ID */
    CFE_SB_PipeId_t  DataPipeId;

    /* Task-related */

    /** \brief Task Run Status */
    uint32  uiRunStatus;

    /* Config table-related */

    /** \brief Config Table Handle */
    CFE_TBL_Handle_t  ConfigTblHdl;

    /** \brief Config Table Pointer */
    CI_ConfigTblEntry_t*  ConfigTblPtr;

    /* Critical Data Storage (CDS) table-related */

    /** \brief CDS Table Handle */
    CFE_ES_CDSHandle_t  CdsTblHdl;

    /** \brief CDS Table data */
    CI_CdsTbl_t  CdsTbl;

    /* Inputs/Outputs */

    /** \brief Input Data from I/O or other apps */
    CI_InData_t   InData;

    /** \brief Output Data published at the end of cycle */
    CI_OutData_t  OutData;

    /** \brief Housekeeping Telemetry for downlink */
    CI_HkTlm_t  HkTlm;

    /** \brief  */
    uint32          ListenerTaskID;

    /** \brief  */
    int32           ListenerTaskRunStatus;

    /** \brief  */
    uint8           IngestBuffer[CI_MAX_CMD_INGEST];

    /** \brief  */
    CFE_SB_Msg_t    *IngestPointer;

    /** \brief  */
    boolean			IngestActive;

    /** \brief  */
    CI_BEHAVIOR		IngestBehavior;

} CI_AppData_t;

/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Prototypes
*************************************************************************/

/************************************************************************/
/** \brief CFS Command Ingest Task (CI) application entry point
**
**  \par Description
**       CFS Command Ingest Task application entry point.  This function
**       performs app initialization, then waits for the cFE ES Startup
**       Sync, then executes the RPR main processing loop.
**
**  \par Assumptions, External Events, and Notes:
**       If there is an unrecoverable failure during initialization the
**       main loop is never executed and the application will exit.
**
*************************************************************************/
void  CI_AppMain(void);

/************************************************************************/
/** \brief Initialize the CFS Command Ingest (CI) application
**
**  \par Description
**       Command Ingest application initialization routine. This
**       function performs all the required startup steps to
**       initialize (or restore from CDS) CI data structures and get
**       the application registered with the cFE services so it can
**       begin to receive command messages and send events.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS    \endcode
**  \retstmt Return codes from #CFE_ES_RegisterApp          \endcode
**  \retstmt Return codes from #CI_InitEvent               \endcode
**  \retstmt Return codes from #CI_InitPipe                \endcode
**  \retstmt Return codes from #CI_InitData                \endcode
**  \retstmt Return codes from #CI_InitConfigTbl           \endcode
**  \retstmt Return codes from #CI_InitCdsTbl              \endcode
**  \retstmt Return codes from #OS_TaskInstallDeleteHandler \endcode
**  \endreturns
**
*************************************************************************/
int32  CI_InitApp(void);

/************************************************************************/
/** \brief Initialize Event Services and Event tables
**
**  \par Description
**       This function performs the steps required to setup
**       cFE Event Services for use by the CI application.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
**  \retstmt Return codes from #CFE_EVS_Register  \endcode
**  \endreturns
**
*************************************************************************/
int32  CI_InitEvent(void);

/************************************************************************/
/** \brief Initialize global variables used by CI application
**
**  \par Description
**       This function performs the steps required to initialize
**       the CI application data.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
**  \retstmt Return codes from #CFE_EVS_Register  \endcode
**  \endreturns
**
*************************************************************************/
int32  CI_InitData(void);

/************************************************************************/
/** \brief Initialize message pipes
**
**  \par Description
**       This function performs the steps required to setup
**       initialize the cFE Software Bus message pipes and subscribe to
**       messages for the CI application.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
**  \retstmt Return codes from #CFE_SB_CreatePipe        \endcode
**  \retstmt Return codes from #CFE_SB_SubscribeEx       \endcode
**  \retstmt Return codes from #CFE_SB_Subscribe         \endcode
**  \endreturns
**
*************************************************************************/
int32  CI_InitPipe(void);

/************************************************************************/
/** \brief Command Ingest Task (CI) cleanup prior to exit
**
**  \par Description
**       This function handles any necessary cleanup prior
**       to application exit.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  CI_CleanupCallback(void);

/************************************************************************/
/** \brief Receive and process messages
**
**  \par Description
**       This function receives and processes messages
**       for the CI application
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   iBlocking    A #CFE_SB_PEND_FOREVER, #CFE_SB_POLL or
**                             millisecond timeout
**
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
**  \retstmt Return codes from #CFE_SB_RcvMsg            \endcode
**  \endreturns
**
*************************************************************************/
int32  CI_RcvMsg(int32 iBlocking);


/************************************************************************/
/** \brief Command Ingest Task incoming data processing
**
**  \par Description
**       This function processes incoming data subscribed
**       by CI application
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  CI_ProcessNewData(void);

/************************************************************************/
/** \brief Command Ingest Task incoming command processing
**
**  \par Description
**       This function processes incoming commands subscribed
**       by CI application
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  CI_ProcessNewCmds(void);

/************************************************************************/
/** \brief Command Ingest Task application commands
**
**  \par Description
**       This function processes command messages
**       specific to the CI application
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   MsgPtr       A #CFE_SB_Msg_t pointer that
**                             references the software bus message
**
*************************************************************************/
void  CI_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr);


/************************************************************************/
/** \brief Sends CI housekeeping message
**
**  \par Description
**       This function sends the housekeeping message
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  CI_ReportHousekeeping(void);

/************************************************************************/
/** \brief Sends CI output data
**
**  \par Description
**       This function publishes the CI application output data.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void  CI_SendOutData(void);

/************************************************************************/
/** \brief Verify Command Length
**
**  \par Description
**       This function verifies the command message length.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   MsgPtr        A #CFE_SB_Msg_t pointer that
**                              references the software bus message
**  \param [in]   usExpectedLen The expected length of the message
**
**  \returns
**  TRUE if the message length matches expectations, FALSE if it does not.
**  \endreturns
**
*************************************************************************/
boolean  CI_VerifyCmdLength(CFE_SB_Msg_t* MsgPtr, uint16 usExpectedLen);

/* TODO:  Add Doxygen markup. */
int32  CI_InitListenerTask(void);
void CI_ListenerTaskMain(void);

/************************************************************************/
/** \brief Validate Command
**
**  \par Description
**       This function validates several parameters of the command.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   MsgPtr        A #CFE_SB_Msg_t pointer that
**                              references the software bus message
**  \param [in]   MsgSize 		The size of the message from the
**  							ingest buffer
**
**  \returns
**  TRUE if the command is valid, FALSE if it is not.
**  \endreturns
**
*************************************************************************/
boolean CI_ValidateCmd(CFE_SB_Msg_t* MsgPtr, uint32 MsgSize);

/************************************************************************/
/** \brief Get Command Authorization
**
**  \par Description
**       This function verifies if a command is authorized to execute
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   MsgPtr        A #CFE_SB_Msg_t pointer that
**                              references the software bus message
**
**  \returns
**  TRUE if the command is authorized, FALSE if it is not.
**  \endreturns
**
*************************************************************************/
boolean CI_GetCmdAuthorized(CFE_SB_Msg_t* MsgPtr);

/************************************************************************/
/** \brief Log Command
**
**  \par Description
**       This function logs execution of the command
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   MsgPtr        A #CFE_SB_Msg_t pointer that
**                              references the software bus message
**
*************************************************************************/
void CI_LogCmd(CFE_SB_Msg_t* MsgPtr);

/************************************************************************/
/** \brief Process Timeouts
**
**  \par Description
**       This function decrements all authorized commands
**       timeout counters and resets their state to unauthorized
**       when it hits zero.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
*************************************************************************/
void CI_ProcessTimeouts(void);

/************************************************************************/
/** \brief Command Authorize
**
**  \par Description
**       This function authorizes a 2-step command.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   msgID        A #CFE_SB_MsgId_t that specifies the
**  						   message ID if of the command
**
**  \param [in]   cmdCode      A #uint16 that specifies the command
**  						   code of the message to authorize
**
*************************************************************************/
void CI_CmdAuthorize(CFE_SB_MsgId_t msgID, uint16 cmdCode);

/************************************************************************/
/** \brief Command Authorize
**
**  \par Description
**       This function deauthorizes a 2-step command.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   msgID        A #CFE_SB_MsgId_t that specifies the
**  						   message ID if of the command
**
**  \param [in]   cmdCode      A #uint16 that specifies the command
**  						   code of the message to deauthorize
**
*************************************************************************/
void CI_CmdDeauthorize(CFE_SB_MsgId_t msgID, uint16 cmdCode);

/************************************************************************/
/** \brief Get Registered Command
**
**  \par Description
**       This function searches for and returns a command
**       from the config table
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \param [in]   msgID        A #CFE_SB_MsgId_t that specifies the
**  						   message ID if of the command
**
**  \param [in]   cmdCode      A #uint16 that specifies the command code
**
**  \returns
**  #CI_CmdData_t if the command is registerd, NULL if it is not.
**  \endreturns
**
*************************************************************************/
CI_CmdData_t *CI_GetRegisterdCmd(CFE_SB_MsgId_t msgID, uint16 cmdCode);

#ifdef __cplusplus
}
#endif 

#endif /* CI_APP_H */

/************************/
/*  End of File Comment */
/************************/
