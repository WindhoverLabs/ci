#############################################################################
#
#   Copyright (c) 2017 Windhover Labs, L.L.C. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
# 3. Neither the name Windhover Labs, L.L.C. nor the names of its 
#    contributors may be used to endorse or promote products derived 
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
#############################################################################
 
set(CI_PROTOBUF_MSGS_DIR ${CMAKE_CURRENT_BINARY_DIR}/msgs/protobuf)

add_airliner_app_def(ci
    FILE CI
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_cds_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_cds_utils.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_config_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_config_utils.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_custom.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_private_ids.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_private_types.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_version.h
        ${CI_PROTOBUF_MSGS_DIR}/ci_auth.pb.c
        ${CI_PROTOBUF_MSGS_DIR}/ci_auth.pb.h
        ${CI_PROTOBUF_MSGS_DIR}/ci_reg.pb.c
        ${CI_PROTOBUF_MSGS_DIR}/ci_reg.pb.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_serialization.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_serialization.c
   
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
		${CI_PROTOBUF_MSGS_DIR}

    PROTOBUF_DEFS
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/ci_reg.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/../msgs/proto_defs/ci_auth.proto

    PROTOBUF_MSGS_DIR        
        ${CI_PROTOBUF_MSGS_DIR}
        
    USER_DOCS   ${CMAKE_CURRENT_SOURCE_DIR}/user_doxy.in   
    DESIGN_DOCS   ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in    
)



add_airliner_app_unit_test(ci-ut
    UTASSERT
	NANOPB
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ci_mock_custom.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_cds_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_cds_utils.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_config_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_config_utils.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_private_ids.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_private_types.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_tbldefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_version.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ci_app_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ci_app_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ci_cds_tbl_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ci_cds_tbl_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ci_mock_custom.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ci_cmds_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ci_cmds_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ci_config_tbl_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ci_config_tbl_test.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ci_test_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ci_test_utils.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ci_testrunner.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../pblib/fsw/src/pb_lib.c
        ${CI_PROTOBUF_MSGS_DIR}/ci_auth.pb.c
        ${CI_PROTOBUF_MSGS_DIR}/ci_auth.pb.h
        ${CI_PROTOBUF_MSGS_DIR}/ci_reg.pb.c
        ${CI_PROTOBUF_MSGS_DIR}/ci_reg.pb.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_serialization.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_serialization.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ci_wrap_pbl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ci_wrap_pbl.h

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/      
        ${CI_PROTOBUF_MSGS_DIR}

	WRAPPERS
		PBLIB_GetDeserializationFunc
)

set(CI_PROTOBUF_MSGS_DIR ${CI_PROTOBUF_MSGS_DIR} PARENT_SCOPE)
