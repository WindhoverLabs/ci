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

#ifndef _CI_EVENTS_H_
#define _CI_EVENTS_H_


/* Event message ID's */
#define CI_EVM_RESERVED							0

#define CI_INIT_INF_EID							1
#define CI_CCIDPIPE_ERR_EID						2
#define CI_ENA_INF_EID							3
#define CI_SUBSCRIBE_ERR_EID					4
#define CI_TLMPIPE_ERR_EID						5
#define CI_SOCKET_ERR_EID						6
#define CI_TLMOUTSTOP_ERR_EID					7
#define CI_MSGID_ERR_EID						8
#define CI_FNCODE_ERR_EID						9
#define CI_ADDPKT_ERR_EID         				10
#define CI_REMOVEPKT_ERR_EID      				11
#define CI_REMOVEALLPTKS_ERR_EID  				12
#define CI_REMOVECMDTO_ERR_EID    				13
#define CI_REMOVEHKTO_ERR_EID     				14
#define CI_ADDPKT_INF_EID         				15
#define CI_REMOVEPKT_INF_EID      				16
#define CI_REMOVEALLPKTS_INF_EID  				17
#define CI_NOOP_INF_EID           				18
#define CI_UPDATE_CLCW_ERR_EID					19
#define CI_PACKET_LOST_ERR_EID    				20
#define CI_SET_FD_EID             				21
#define CI_CONFIG_REG_ERR_EID      				22
#define CI_CONFIG_LD_ERR_EID       				23
#define CI_CONFIG_MNG_ERR_EID      				24
#define CI_CONFIG_GADR_ERR_EID     				25
#define CI_CONFIG_INF_EID          				26

#define CI_CMDINSOCKET_ERR_EID					27
#define CI_CMDSER_ERR_EID						28
#define CI_CMD_INGEST_ERR_EID					29
#define CI_CMD_TRANSFER_FRAME_FAILED_EID		30
#define CI_SEQ_NOT_FOUND_EID					31
#define CI_ERR_BAD_CODE_BLOCK_EID				32
#define CI_ERR_TF_NEGATIVE_SLIDING_EDGE_EID		33
#define CI_ERR_TF_POSITIVE_SLIDING_EDGE_EID 	34
#define CI_INVALID_TRANSFER_FRAME_LENGTH_EID 	35
#define CI_ERR_TF_LOCKOUT_MODE_EN_EID 			36
#define CI_TF_CMD_REJECTED_LOCKOUT_MODE_EN_EID 	37
#define CI_ERR_PKT_CONSTRUCT_FAILURE_EID 		38
#define CI_ERR_PKT_RECONSTRUCT_EID 				39
#define CI_ERR_CHECKSUM_EID 					40
#define CI_TRANSFER_FRAME_DISREGARDED_EID 		41
#define CI_TRANSFER_LAYER_UNLOCK_EID 			42
#define CI_TRANSFER_FRAME_CNT_RESET_EID 		43
#define CI_RESYNC_CMD_IGNORE_EID 				44
#define CI_INV_BYPASS_CMD_EID 					45
#define CI_TLMTASK_CREATE_ERR_EID 				46

#define CI_RCVD_UNDEFINED_CMD_EID				47
#define CI_RCVD_INHIBITED_CMD_EID				48
#define CI_RCVD_NOT_SUPPORTED_CMD_EID			49
#define CI_RCVD_TWO_STEP_NOT_ARMED_CMD_EID 		50
#define CI_ARM_ERR_EID							51
#define CI_ARM_INF_EID							52
#define CI_DISARM_ERR_EID						53
#define CI_DISARM_INF_EID						54
#define CI_ARM_CMD_TIMEOUT_EID					55

#define CI_LISTENER_CREATE_CHDTASK_ERR_EID		56


#endif  /* _CI_EVENTS_H_ */
