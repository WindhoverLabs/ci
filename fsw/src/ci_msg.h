    
#ifndef CI_MSG_H
#define CI_MSG_H

/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** CI Command Codes
*************************************************************************/

/** \cicmd Noop 
**  
**  \par Description
**       Implements the Noop command that demonstrates the CI task is alive
**
**  \cicmdmnemonic \CI_NOOP
**
**  \par Command Structure
**       #CI_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \CI_CMDACPTCNT - command counter will increment
**       - The #CI_CMD_INF_EID informational event message will be 
**         generated when the command is received
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \CI_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #CI_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #CI_RESET_CC
*/
#define CI_NOOP_CC                 (0)

/** \cicmd Reset Counters
**  
**  \par Description
**       Resets the ci housekeeping counters
**
**  \cicmdmnemonic \CI_TLMRST
**
**  \par Command Structure
**       #CI_NoArgCmd_t
**
**  \par Command Verification
**       Successful execution of this command may be verified with
**       the following telemetry:
**       - \b \c \CI_CMDACTPCNT       - command counter will be cleared
**       - \b \c \CI_CMDRJCTCNT       - command error counter will be cleared
**       - The #CI_CMD_INF_EID debug event message will be 
**         generated when the command is executed
** 
**  \par Error Conditions
**       This command may fail for the following reason(s):
**       - Command packet length not as expected
** 
**  \par Evidence of failure may be found in the following telemetry: 
**       - \b \c \CI_CMDRJCTCNT - command error counter will increment
**       - Error specific event message #CI_MSGID_ERR_EID
**
**  \par Criticality
**       None
**
**  \sa #CI_NOOP_CC
*/
#define CI_RESET_CC                (1)

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/** 
**  \brief No Arguments Command
**  For command details see #CI_NOOP_CC, #CI_RESET_CC
**  Also see #CI_SEND_HK_MID
*/
typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} CI_NoArgCmd_t;

/** 
**  \brief TODO Elaborate this struct
**  Boilerplate example of application-specific incoming data
*/
typedef struct
{
    uint8   TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32  counter;

    /* TODO:  Add input data to this application here, such as raw data read from I/O
    **        devices.
    **        Option: for data that is already defined by another app, include
    **        that app's message header above.
    */

} CI_InData_t;

/** 
**  \brief TODO Elaborate this struct
**  Boilerplate example of application-specific outgoing data
*/
typedef struct
{
    uint8   ucTlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32  uiCounter;
} CI_OutData_t;

/** 
**  \brief CI application housekeeping data
*/
typedef struct
{
    /** \brief cFE SB Tlm Msg Hdr */
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];

    /** \citlmmnemonic \CI_CMDACPTCNT
        \brief Count of accepted commands */
    uint8              usCmdCnt;   

    /** \citlmmnemonic \CI_CMDRJCTCNT
        \brief Count of failed commands */
    uint8              usCmdErrCnt; 

    /** \brief Padding for struct alignment */
    uint8              padding[2];  
  
    /* TODO:  Add Doxygen markup. */
    uint8              IngestMsgCount;

    /* TODO:  Add Doxygen markup. */
    uint8              IngestErrorCount;

} CI_HkTlm_t;


#ifdef __cplusplus
}
#endif

#endif /* CI_MSG_H */

/************************/
/*  End of File Comment */
/************************/
