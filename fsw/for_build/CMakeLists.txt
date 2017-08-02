
add_airliner_app_def(ci
    FILE CI
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_cds_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_config_utils.c
   
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/

    USER_DOCS   ${CMAKE_CURRENT_SOURCE_DIR}/user_doxy.in
)



add_airliner_app_unit_test(CI-UT
    UTF 
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ci_app_test
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ci_cds_tbl_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ci_cmds_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ci_config_tbl_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ci_test_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/ci_testrunner.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_cds_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_config_utils.c
        
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../unit_test/
)