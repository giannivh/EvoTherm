cmake_minimum_required(VERSION 3.6)
project(evotherm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        ext/jsoncpp/jsoncpp.cpp ext/jsoncpp/json/json.h ext/jsoncpp/json/json-forwards.h
        main.cpp
        model/exitcode.h
        model/markup.h
        model/app.h
        model/config.cpp model/config.h
        model/io/httpclient.cpp model/io/httpclient.h
        model/io/cmdparser.cpp model/io/cmdparser.h
        model/io/terminal.cpp model/io/terminal.h
        model/evohome/evohomeclient.cpp model/evohome/evohomeclient.h
        model/evohome/data/userinfo.cpp model/evohome/data/userinfo.h
        model/evohome/data/installationinfo.cpp model/evohome/data/installationinfo.h
        model/evohome/data/zone.cpp model/evohome/data/zone.h
        model/evohome/data/mode.cpp model/evohome/data/mode.h)

add_executable(evotherm ${SOURCE_FILES})
target_link_libraries(evotherm curl)