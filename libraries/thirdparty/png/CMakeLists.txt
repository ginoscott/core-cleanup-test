#cmake_minimum_required(VERSION 2.8.12)

include_directories(${COSMOS_SOURCE}/core/libraries/thirdparty/zlib)

FILE(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
FILE(GLOB INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(png STATIC ${SOURCES} ${INCLUDES})

install(TARGETS png DESTINATION lib)
install(FILES ${INCLUDES} DESTINATION include/thirdparty/png)


if(${DEPLOY_SOURCE_FILES_TO_REMOTE} MATCHES "yes")
    add_deployment_source_directory(${CMAKE_CURRENT_SOURCE_DIR} "src")
endif()
