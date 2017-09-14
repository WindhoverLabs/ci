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
        #${CI_PROTOBUF_MSGS_DIR}/ci_auth.pb.c
        #${CI_PROTOBUF_MSGS_DIR}/ci_auth.pb.h
        #${CI_PROTOBUF_MSGS_DIR}/ci_reg.pb.c
        #${CI_PROTOBUF_MSGS_DIR}/ci_reg.pb.h
   
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

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/      
        ${CI_PROTOBUF_MSGS_DIR}
)

set(CI_PROTOBUF_MSGS_DIR ${CI_PROTOBUF_MSGS_DIR} PARENT_SCOPE)
