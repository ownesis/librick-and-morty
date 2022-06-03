cmake_minimum_required(VERSION 3.0)

project(librick-and-morty VERSION 0.1.0 DESCRIPTION "C library for https://rickandmortyapi.com")

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
include(GNUInstallDirs)

set_target_properties(rick-and-morty PROPERTIES PUBLIC_HEADER api.h)

install(TARGETS rick-and-morty
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

find_package(CURL)
find_package(JSON-C NAMES json-c)

if(CURL_FOUND)
  # Une fois la dépendance trouvée, nous l'incluons au projet
  target_include_directories(rick-and-morty PUBLIC ${CURL_INCLUDE_DIR})
  target_link_libraries(rick-and-morty ${CURL_LIBRARY})
else()
  # Sinon, nous affichons un message
  message(FATAL_ERROR "libcurl not found")
endif()

if(JSON-C_FOUND)
  # Une fois la dépendance trouvée, nous l'incluons au projet
  target_include_directories(rick-and-morty PUBLIC ${JSON-C_INCLUDE_DIR})
  target_link_libraries(rick-and-morty ${JSON-C_LIBRARY})
else()
  # Sinon, nous affichons un message
  message(FATAL_ERROR "libjson-c not found")
endif()
