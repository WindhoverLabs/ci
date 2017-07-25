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

#ifndef _CI_APP_H_
#define _CI_APP_H_

/*
** Pragmas
*/

/*
** Include Files
*/

#include "common_types.h"
#include "cfe_time.h"
#include "cfe_evs.h"
#include "cfe_es.h"
#include "cfe_sb.h"

#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "cmds.h"
#include "osapi.h"
#include "msg.h"
#include "tbl.h"

/*
** Local Defines
*/

//#define CI_DEBUG
#define CI_UNUSED              0
#define CI_EVT_COUNT           8
#define CI_MAX_SER_STRING_SIZE 11
#define CI_CMD_PIPE_DEPTH      20
#define CI_SCH_PIPE_DEPTH      10

#define CI_MAX_CMD_INGEST      (CFE_SB_MAX_SB_MSG_SIZE)
#define CI_CMD_PORT            5010

#define CI_CMD_LOG_NONE        0
#define CI_CMD_LOG_DEFAULT     0

#define CI_LISTENER_TASK_NAME  		"CI_LISTENER"
#define CI_LISTENER_TASK_STACK_SIZE	16000
#define CI_LISTENER_TASK_PRIORITY	100


/*
** Local Structure Declarations
*/
typedef struct
{
    uint8   Octet1;
    uint16  PDataLen;
    uint8   Octet4;
    uint16  SrcEntityId;
    uint32  TransSeqNum;
    uint16  DstEntityId;

}OS_PACK CF_PDU_Hdr_t;

typedef struct
{
   uint8       		TlmHeader[CFE_SB_TLM_HDR_SIZE];
   uint16      		CmdCount;
   uint16      		CmdErrCount;
   uint32      		IngestMsgCount;
   uint32      		IngestErrorCount;
   uint8       		FDPdusDropped;
   uint8       		EOFPdusDropped;
   uint8       		FINPdusDropped;
   uint8       		ACKPdusDropped;
   uint8       		MDPdusDropped;
   uint8       		NAKPdusDropped;
   uint8       		Spare[2];
   uint32      		PDUsCaptured;
   uint32      		PktSubCount;
   CI_SchedMode_t	SchedMode;
} CI_HkPacket_T;

#define CI_HK_TLM_LNGTH      sizeof(CI_HkPacket_T)

typedef struct
{
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint16             synch;

    uint16             bit1:1;
    uint16             bit2:1;
    uint16             bit34:2;
    uint16             bit56:2;
    uint16             bit78:2;
    uint16             nibble1:4;
    uint16             nibble2:4;

    uint8              bl1, bl2;       /* boolean */
    int8               b1, b2, b3, b4;
    int16              w1,w2;
    
    int32              dw1, dw2;
    float              f1, f2;
    double             df1, df2;
    char               str[10];
}CI_Data_Types_Fmt_T;

#define CI_DATA_TYPES_LNGTH   sizeof (CI_Data_Types_Fmt_T)

/* Type definition (TO app global data)...  */
typedef struct
{
   CI_HkPacket_T           HkPacket;
   CI_Data_Types_Fmt_T     DataTypesPacket;

   CFE_SB_MsgPtr_t          MsgPtr;
   CFE_SB_PipeId_t          CmdPipe;
   CFE_SB_PipeId_t          SchPipe;

   uint32                   RunStatus;

   char                     CmdPipeName[16];
   uint16                   CmdPipeDepth;
   char                     SchPipeName[16];
   uint16                   SchPipeDepth;

   CFE_EVS_BinFilter_t      EventFilters[CI_EVT_COUNT];

   CFE_TBL_Handle_t         ConfigHandle;
   CI_ConfigTable_t*        Config;

   uint8                    IngestBuffer[CI_MAX_CMD_INGEST];
   CFE_SB_Msg_t             *IngestPointer;
   char                     TlmDestSerialPort[CI_MAX_SER_STRING_SIZE];

   boolean                  DropFileData;
   int32                    DropFileDataCnt;
   boolean                  DropEOF;
   int32                    DropEOFCnt;
   boolean                  DropFIN;
   int32                    DropFINCnt;
   boolean                  DropACK;
   int32                    DropACKCnt;
   boolean                  DropMetaData;
   int32                    DropMetaDataCnt;
   boolean                  DropNAK;
   int32                    DropNAKCnt;
   boolean                  CorruptChecksum;
   boolean                  AdjustFileSize;
   int32                    PDUFileSizeAdjustment;
   uint32					ListenerTaskID;
   int32   					ListenerTaskRunStatus;
} CI_AppData_T;

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
int32 CI_AppInit(void);
void  CI_Cleanup(void);
void  CI_ProcessCmdMsg(void);
void  CI_ProcessSchMsg(void);
void  CI_OutputStatus(void);
void  CI_AppMain(void);
int32 CI_TableInit (void);
int32 CI_ValidateTable (CI_ConfigTable_t*);
int32 CI_RecvMsg( int32 timeout);
void  CI_ReadSyncUpLink(void);
void  CI_ProcessPDU(void);
int32 CI_InitListenerTask(void);
void  CI_ListenerTaskMain(void);
int32 CI_InitIncomingChannels(void);


#endif  /* _CI_APP_H_ */

/* ---------- end of file to_app.h ----------*/
