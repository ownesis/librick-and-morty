cmake_minimum_required(VERSION 3.0)

project(librick-and-morty)

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

add_library(rick-and-morty STATIC ${SRCS} ${HRDS})

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
