cmake_minimum_required(VERSION 3.0)

project(librick-and-morty VERSION 0.1.0 DESCRIPTION "C library for https://rickandmortyapi.com/")

set(SRCS
    character.c
    episode.c
    location.c
    api.c
    utils.c
    )

set(HDRS
    character.h
    episode.h
    location.h
    api.h
    utils.h
    types.h
    )

add_library(rick-and-morty SHARED ${SRCS} ${HRDS})
set_target_properties(rick-and-morty PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(rick-and-morty PROPERTIES SOVERSION ${PROJECT_VERSION})

add_library(rick-and-morty_static STATIC ${SRCS} ${HRDS})
set_target_properties(rick-and-morty_static PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(rick-and-morty_static PROPERTIES SOVERSION ${PROJECT_VERSION})

include(GNUInstallDirs)

install(TARGETS rick-and-morty
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS rick-and-morty_static
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS rick-and-morty
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

find_package(CURL REQUIRED)
find_package(JSON-C CONFIG REQUIRED)

if(CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIR})
    target_link_libraries(rick-and-morty PRIVATE ${CURL_LIBRARY})
    target_link_libraries(rick-and-morty_static PRIVATE ${CURL_LIBRARY})
else()
    message(FATAL_ERROR "libcurl not found")
endif()
if(JSON-C_FOUND)
    target_link_libraries(rick-and-morty PRIVATE json-c::json-c)
    target_link_libraries(rick-and-morty_static PRIVATE json-c::json-c)
else()
    message(FATAL_ERROR "json-c not found")
endif()
