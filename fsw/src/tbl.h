/*==============================================================================
Copyright (c) 2015, Windhover Labs
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of CmdIn nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _CI_TBL_
#define _CI_TBL_

/*
** Pragmas
*/

/*
** Include Files
*/
#include "cfe.h"
#include "ci_mission_cfg.h"
#include "ci_platform_cfg.h"
#include <arpa/inet.h>

#include "app.h"
#include "msg.h"

/*
** Local Defines
*/

#define CI_CONFIG_TABLENAME    "ci_config"

typedef enum
{
	CI_CHANNEL_UNUSED 			= 0,
	CI_CHANNEL_DISABLED 		= 1,
	CI_CHANNEL_ENABLED 			= 2
} CI_ChannelMode_t;


/*
** Local Structure Declarations
*/


typedef struct
{
	uint32				Port;
	int32				Socket;
} CI_AsyncChannel_t;


typedef struct
{
	CI_ChannelMode_t	Mode;
	uint32				Port;
	int32				Socket;
} CI_SyncChannel_t;


typedef struct
{
    uint32				PDUMessageID;
    CFE_SB_TimeOut_t	Timeout;
    CI_AsyncChannel_t	AsyncChannel;
    CI_SyncChannel_t	SyncChannel[CI_MAX_SYNC_CHANNELS];
} CI_ConfigTable_t;

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


#endif
