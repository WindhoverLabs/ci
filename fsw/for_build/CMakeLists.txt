
add_airliner_app_def(ci
    FILE CI
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_cds_utils.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ci_config_utils.c
   
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
)



