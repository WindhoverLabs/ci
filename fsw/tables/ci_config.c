/*==============================================================================
** File Name: ci_iloads.c
**
** Title:     CI iloads table definition
**
** $Author: $
** $Revision: $
** $Date:  $
**
** Purpose:   To provide the CI iloads table for default data config.
**
** Functions Contained:
**    None
**
**
** Limitations, Assumptions, External Events, and Notes:
**  1.   None
**
** Modification History:
**   MM/DD/YY  SCR/SDR     Author          DESCRIPTION
**   --------  ----------  -------------   -----------------------------
**   mm/dd/yy  $$$SCRxxxx  C. Writer       Build #: Code Started
**
**
**==============================================================================
*/

#ifndef _CI_CONFIG_
#define _CI_CONFIG_

#ifdef	__cplusplus
extern "C" {
#endif


/*
** Pragmas
*/

/*
** Include Files
*/
#include "app.h"
#include "tbl.h"
#include "cfe_tbl_filedef.h"
#include "ci_platform_cfg.h"
#include "ci_mission_cfg.h"
#include "msg_ids.h"
#include "cfe_es_msg.h"
#include "cmd_codes.h"

/*
** Local Defines
*/


/*
** Local Structure Declarations
*/

CFE_TBL_FileDef_t CFE_TBL_FileDef =
{
    "ci_ConfigTable", "CI.ci_config", "CI config table",
    "ci_config.tbl", sizeof(CI_ConfigTable_t)
};

/*
** Default CI iLoad table data
*/


CI_ConfigTable_t ci_ConfigTable =
{
	0x1ffd,
	1000,
	CI_CMD_TYPE_ONE_STEP,
	CI_CMD_LOG_NONE,
	{
		{ CI_CHANNEL_ENABLED, CI_CHANNEL_TYPE_UDP, "0.0.0.0", TRUE, FALSE, 0, 5010, 0, 0},
		{ CI_CHANNEL_ENABLED, CI_CHANNEL_TYPE_UDP, "0.0.0.0", FALSE, TRUE, FIXHAWK_MAVLINK_IN_MSG_MID, 14555, 0, 0}
	},
	{
		{ CI_CMD_MID, CI_NOP_CC, 			CI_CMD_TYPE_ONE_STEP, {0,0}, 200, CI_CMD_LOG_DEFAULT, CI_CMD_STATE_NA, {0,0} },
		{ CI_CMD_MID, CI_RESET_STATUS_CC, 	CI_CMD_TYPE_ONE_STEP, {0,0}, 201, CI_CMD_LOG_DEFAULT, CI_CMD_STATE_NA, {0,0} },
		{ CI_CMD_MID, CI_CMD_ARM, 			CI_CMD_TYPE_ONE_STEP, {0,0}, 202, CI_CMD_LOG_DEFAULT, CI_CMD_STATE_NA, {0,0} },
		{ CI_CMD_MID, CI_CMD_DISARM, 		CI_CMD_TYPE_ONE_STEP, {0,0}, 203, CI_CMD_LOG_DEFAULT, CI_CMD_STATE_NA, {0,0} },
		{ FIXHAWK_MAVLINK_IN_MSG_MID,  0,	CI_CMD_TYPE_ONE_STEP, {0,0}, 203, CI_CMD_LOG_NONE,    CI_CMD_STATE_NA, {0,0} }
	}
};

/*
** External Global Variables
*/

/*
** Global Variables
*/

/*
** Local Variables
*/

/*
** Local Function Prototypes
*/

#ifdef	__cplusplus
}
#endif

#endif /* _CI_CONFIG_ */
