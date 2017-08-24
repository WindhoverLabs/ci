#include "ci_app.h"
#include "cfe.h"

typedef enum
{
	SOCK_ERR,
	BIND_ERR,
	NOMINAL
} CI_InitStatus_t;

typedef enum
{
	CI_CMD,
	EXT_STEP_1,
	EXT_STEP_2
} CI_RecvMsgs_t;

/* Return codes */
extern int INIT_CUSTOM_RET;
extern int READ_MSG_RET;

/* Return codes */
CFE_SB_MsgPtr_t TestMsgPtr;

/* Premade buffers */
extern uint8 cmd_buffer[CI_MAX_CMD_INGEST];
extern uint8 bad_ccsds_buf[CI_MAX_CMD_INGEST];
extern uint8 no_sec_hdr_buf[CI_MAX_CMD_INGEST];
extern uint8 tlm_buf[CI_MAX_CMD_INGEST];
extern uint8 checksum_buf[CI_MAX_CMD_INGEST];
extern uint8 inv_checksum_buf[CI_MAX_CMD_INGEST];
