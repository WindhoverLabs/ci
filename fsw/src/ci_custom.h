
#ifndef CI_CUSTOM_H
#define CI_CUSTOM_H

#include "cfe.h"

int32 CI_InitCustom(void);
int32 CI_ReadMessage(const char* buffer, uint32* size);
int32 CI_CleanupCustom(void);

#endif
