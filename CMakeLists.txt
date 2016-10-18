cmake_minimum_required(VERSION 3.6)
project(evotherm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        src/ext/jsoncpp/jsoncpp.cpp src/ext/jsoncpp/json/json.h src/ext/jsoncpp/json/json-forwards.h
        src/main.cpp
        src/model/exitcode.h
        src/model/markup.h
        src/model/app.h
        src/model/config.cpp src/model/config.h
        src/model/io/httpclient.cpp src/model/io/httpclient.h
        src/model/io/cmdparser.cpp src/model/io/cmdparser.h
        src/model/io/terminal.cpp src/model/io/terminal.h
        src/model/evohome/evohomeclient.cpp src/model/evohome/evohomeclient.h
        src/model/evohome/data/userinfo.cpp src/model/evohome/data/userinfo.h
        src/model/evohome/data/installationinfo.cpp src/model/evohome/data/installationinfo.h
        src/model/evohome/data/zone.cpp src/model/evohome/data/zone.h
        src/model/evohome/data/mode.cpp src/model/evohome/data/mode.h
        src/model/util/dateutil.cpp src/model/util/dateutil.h
        src/model/exception/evothermexception.cpp src/model/exception/evothermexception.h)

# Find curl
# Find the native CURL headers and libraries
#
#  CURL_INCLUDE_DIRS - where to find curl/curl.h, etc
#  CURL_LIBRARIES    - list of libraries when using curl
#  CURL_FOUND        - true if curl found.

# look for the header file
FIND_PATH(CURL_INCLUDE_DIR curl/curl.h
        $ENV{INCLUDE}
        "$ENV{LIB_DIR}/include"
        /usr/local/include
        /usr/include
        #mingw
        c:/msys/local/include
        NO_DEFAULT_PATH
        )

MARK_AS_ADVANCED(CURL_INCLUDE_DIR)

# look for the library
FIND_LIBRARY(CURL_LIBRARY NAMES curl libcurl_imp PATHS
        $ENV{LIB}
        "$ENV{LIB_DIR}/lib"
        /usr/local/lib
        /usr/lib
        c:/msys/local/lib
        NO_DEFAULT_PATH
        )

MARK_AS_ADVANCED(CURL_LIBRARY)

# report
IF(CURL_INCLUDE_DIR)
    MESSAGE(STATUS "Curl include was found")
ENDIF(CURL_INCLUDE_DIR)
IF(CURL_LIBRARY)
    MESSAGE(STATUS "Curl lib was found")
ENDIF(CURL_LIBRARY)

# copy the results to the output variables.
IF(CURL_INCLUDE_DIR AND CURL_LIBRARY)
    SET(CURL_FOUND 1)
    SET(CURL_LIBRARIES ${CURL_LIBRARY})
    SET(CURL_INCLUDE_DIRS ${CURL_INCLUDE_DIR})
ELSE(CURL_INCLUDE_DIR AND CURL_LIBRARY)
    SET(CURL_FOUND 0)
    SET(CURL_LIBRARIES)
    SET(CURL_INCLUDE_DIRS)
ENDIF(CURL_INCLUDE_DIR AND CURL_LIBRARY)

# report the results.
IF(CURL_FOUND)
    IF (NOT CURL_FIND_QUIETLY)
        MESSAGE(STATUS "Found CURL: ${CURL_LIBRARY}")
    ENDIF (NOT CURL_FIND_QUIETLY)
ELSE(CURL_FOUND)
    SET(CURL_DIR_MESSAGE "CURL was not found.")
    IF(CURL_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "${CURL_DIR_MESSAGE}")
    ELSE(CURL_FIND_REQUIRED)
        IF(NOT CURL_FIND_QUIETLY)
            MESSAGE(STATUS "${CURL_DIR_MESSAGE}")
        ENDIF(NOT CURL_FIND_QUIETLY)
        # avoid cmake complaints if CURL is not found
        SET(CURL_INCLUDE_DIR "")
        SET(CURL_LIBRARY "")
    ENDIF(CURL_FIND_REQUIRED)
ENDIF(CURL_FOUND)


add_executable(evotherm ${SOURCE_FILES})
target_link_libraries(evotherm curl)

install(TARGETS evotherm RUNTIME DESTINATION bin)