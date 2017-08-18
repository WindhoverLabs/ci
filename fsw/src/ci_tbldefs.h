    
#ifndef CI_TBLDEFS_H
#define CI_TBLDEFS_H

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
/**
 * \brief Defines the number of entries in the table
 */
#define CI_CONFIG_TABLE_MAX_ENTRIES  (1)

/**
 * \brief Defines the table identification name used for table registration.
 */
#define CI_CONFIG_TABLENAME          ("CONFIG_TBL")

/**
 * \brief Defines the table file name used for table registration.
 */
#define CI_CDS_TABLENAME  ("ci_CdsTbl")

/************************************************************************
** Local Structure Declarations
*************************************************************************/

/** \brief Definition for a single config table entry */
typedef struct
{
	uint8			TableID;
	CI_CmdData_t	cmds[CI_MAX_RGST_CMDS];

} CI_ConfigTblEntry_t;


/** \brief Definition for Critical Data Storage (CDS) table entry */
typedef struct
{
    int32  iParam;

    /* TODO:  Add type declaration for CDS data here. */
} CI_CdsTbl_t;

/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/

#define CI_CMD_1_STEP				0
#define CI_CMD_2_STEP				1
#define CI_CMD_UNAUTHORIZED			0
#define CI_CMD_AUTHORIZED			1
#define CI_CMD_LOG					0
#define CI_CMD_EXCLUDE_LOG			1

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Prototypes
*************************************************************************/


#ifdef __cplusplus
}
#endif

#endif /* CI_TBLDEFS_H */

/************************/
/*  End of File Comment */
/************************/
