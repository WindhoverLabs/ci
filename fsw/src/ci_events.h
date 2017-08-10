    
#ifndef CI_EVENTS_H
#define CI_EVENTS_H

#include <termio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
** Local Defines
*************************************************************************/


/* Event IDs
 * Conventions: _EID is the event identifier.  _EVT_CNT is the total number of
 * events and should always be last.  Events can be added before _EVT_CNT.
 * For long-term maintenance, consider not removing an event but replacing it
 * with an unused, reserved, enum to preserve the IDs later in the list. */
typedef enum {

/** \brief <tt> Value of zero is reserved, and should not be used. </tt> */
    CI_RESERVED_EID = 0,  /* Do not use this event ID */

/** \brief <tt> 'CI - ' </tt>
**  \event <tt> 'CI - ' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  TODO fill this in
**
*/
    CI_INF_EID,

/** \brief <tt> 'CI Initialized. Version \%d.\%d.\%d' </tt>
**  \event <tt> 'CI Initialized. Version \%d.\%d.\%d' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the CFS CI Task has
**  completed initialization.
**
**  The first \c %d field contains the Application's Major Version Number
**  The second \c %d field contains the Application's Minor Version Number
**  The third \c %d field contains the Application's Revision Number
*/
    CI_INIT_INF_EID,

/** \brief <tt> 'CI - ' </tt>
**  \event <tt> 'CI - ' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This message is generated when a table is initialized.
**
*/
    CI_CONFIG_TABLE_INF_EID,

/** \brief <tt> 'CI - ' </tt>
**  \event <tt> 'CI - ' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when TBD
**
*/
    CI_CDS_INF_EID,

/** \brief <tt> 'CI - Recvd $x cmd (%us)' </tt>
**  \event <tt> 'CI - Recvd $x cmd (%us)' </tt>
**  
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event message is issued when the CFS CI Task has
**  received and processed a command.
**
*/
    CI_CMD_INF_EID,

/** \brief <tt> 'CI - ' </tt>
**  \event <tt> 'CI - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when TBD
**
*/
    CI_ERR_EID,

/** \brief <tt> 'CI - ' </tt>
**  \event <tt> 'CI - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS CI Task has
**  had an error in initialization.
**
*/
    CI_INIT_ERR_EID,

/** \brief <tt> 'CI - ' </tt>
**  \event <tt> 'CI - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS CI Task has
**  had an error with the configuration table.
**
*/
    CI_CONFIG_TABLE_ERR_EID,

/** \brief <tt> 'CI - ' </tt>
**  \event <tt> 'CI - ' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS CI Task has
**  had an error in CDS.
**
*/
    CI_CDS_ERR_EID,

/** \brief <tt> 'CI - $commandError' </tt>
**  \event <tt> 'CI - $commandError' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS CI Task has
**  had an error processing a command.
**
*/
    CI_CMD_ERR_EID,

/** \brief <tt> 'CI: SB pipe read error (0x%08X), app will exit' </tt>
**  \event <tt> 'CI: SB pipe read error (0x%08X), app will exit' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS CI Task has
**  had an error reading from a pipe.
**
*/
    CI_PIPE_ERR_EID,

/** \brief <tt> 'CI - Recvd invalid $type msgId (0x%04x)' </tt>
**  \event <tt> 'CI - Recvd invalid $type msgId (0x%04x)' </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS CI Task has
**  received an invalid message ID.
**
*/
    CI_MSGID_ERR_EID,

/** \brief <tt> 'CI - Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, msgLen=%d, expectedLen=%d" </tt>
**  \event <tt> 'CI - Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, msgLen=%d, expectedLen=%d" </tt>
**  
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message is issued when the CFS CI Task has
**  received a message with a bad length.
**
*/
    CI_MSGLEN_ERR_EID,

    /* TODO:  Add Doxygen markup. */
    CI_LISTENER_CREATE_CHDTASK_ERR_EID,
    CI_CMD_INGEST_ERR_EID,
	CI_SOCKET_ERR_EID,
	CI_ENA_INF_EID,

/** \brief <tt> This is a count of all the app events and should not be used. </tt> */
    CI_EVT_CNT
} CI_EventIds_t;


#ifdef __cplusplus
}
#endif

#endif /* CI_EVENTS_H */

/************************/
/*  End of File Comment */
/************************/
    
