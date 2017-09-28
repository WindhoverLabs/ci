/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include <string.h>
#include "ci_app.h"
#include "ci_cds_utils.h"

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/************************************************************************
** External Global Variables
*************************************************************************/
extern CI_AppData_t  CI_AppData;

/************************************************************************
** Global Variables
*************************************************************************/

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Function Prototypes
*************************************************************************/

/************************************************************************
** Function Definitions
*************************************************************************/


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Initialize CDS Tables                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 CI_InitCdsTbl()
{
    int32  iStatus=0;
    int32  iResetType=0;
    uint32 uiResetSubType=0;

    /* Only clear CDS table when it's a PowerOn reset, and not a Processor reset */
    iResetType = CFE_ES_GetResetType(&uiResetSubType);
    if (iResetType == CFE_ES_POWERON_RESET)
    {
        memset((void*)&CI_AppData.CdsTbl, 0x00, sizeof(CI_CdsTbl_t));
    }

    /* Register and manage CDS table */
    iStatus = CFE_ES_RegisterCDS(&CI_AppData.CdsTblHdl, sizeof(CI_CdsTbl_t),
                                 CI_CDS_TABLENAME);
    if (iStatus == CFE_SUCCESS)
    {
        /* Setup initial content of CDS table */
        iStatus = CFE_ES_CopyToCDS(CI_AppData.CdsTblHdl, &CI_AppData.CdsTbl);
        if (iStatus == CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(CI_CDS_INF_EID, CFE_EVS_INFORMATION,
                                     "Successfully setup CDS");
        }
        else
        {
            (void) CFE_EVS_SendEvent(CI_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "Failed to setup CDS");
        }
    }
    else if (iStatus == CFE_ES_CDS_ALREADY_EXISTS)
    {
        /* If one already exists, get a copy of its current content */
        memset((void*)&CI_AppData.CdsTbl, 0x00, sizeof(CI_CdsTbl_t));
        iStatus = CFE_ES_RestoreFromCDS(&CI_AppData.CdsTbl, CI_AppData.CdsTblHdl);
        if (iStatus == CFE_SUCCESS)
        {
            (void) CFE_EVS_SendEvent(CI_CDS_INF_EID, CFE_EVS_INFORMATION,
                                     "Successfully restored data from CDS");
        }
        else
        {
            (void) CFE_EVS_SendEvent(CI_INIT_ERR_EID, CFE_EVS_ERROR,
                                     "Failed to restore data from CDS");
            memset((void*)&CI_AppData.CdsTbl, 0x00, sizeof(CI_CdsTbl_t));
        }
    }
    else
    {
        (void) CFE_EVS_SendEvent(CI_INIT_ERR_EID, CFE_EVS_ERROR,
                                 "Failed to create CDS (0x%08X)", (unsigned int)iStatus);
    }

    return (iStatus);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Update CDS Tables                                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CI_UpdateCdsTbl()
{
    /* TODO:  Add code to update values in CDS table here */
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Save CDS Tables                                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CI_SaveCdsTbl()
{
    /* TODO This return value is not checked. Developer should decide what to do here
       in case of failure or should add a return value for higher-level logic to handle. */
    CFE_ES_CopyToCDS(CI_AppData.CdsTblHdl, &CI_AppData.CdsTbl);
}

/************************/
/*  End of File Comment */
/************************/
    
