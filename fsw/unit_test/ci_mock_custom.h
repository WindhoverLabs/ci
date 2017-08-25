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
	EXT_STEP_2,
	LONG_CMD,
	INV_CMD
} CI_RecvMsgs_t;

/* Return codes */
extern int INIT_CUSTOM_RET;
extern int READ_MSG_RET;
extern int TEST_MSG_ID;
extern int TEST_CC;
