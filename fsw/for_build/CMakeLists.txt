
add_airliner_app_def(ci
    FILE CI
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/cmds.c 
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/crc.c 
   
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
)



