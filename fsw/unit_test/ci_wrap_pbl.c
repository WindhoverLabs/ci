#ifdef __cplusplus
extern "C" {
#endif

#include "../../../pblib/fsw/public_inc/pb_lib.h"
#include "ci_serialization.h"

int PBLIB_RET_CODE = 0;

PBLib_DecodeFuncPtr_t __wrap_PBLIB_GetDeserializationFunc(CFE_SB_MsgId_t msgId, uint16 cmdCode)
{
	if(PBLIB_RET_CODE == 0)
	{
		return 0;
	}

	return CI_CmdRegData_Dec;
}

