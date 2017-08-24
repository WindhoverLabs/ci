
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
   
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        
    USER_DOCS   ${CMAKE_CURRENT_SOURCE_DIR}/user_doxy.in   
    DESIGN_DOCS   ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in    
)



add_airliner_app_unit_test(ci-ut
    UTASSERT 
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ci_mock_custom.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_app.h
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

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
)
