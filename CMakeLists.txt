cmake_minimum_required(VERSION 3.17)

project(json-benchmark CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fPIE -march=native -O2 -Wall" )
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.2 -mavx -O3 -Wall" )

set( CMAKE_BUILD_TYPE Release )
add_compile_definitions(TEST_DATA_PATH="${PROJECT_SOURCE_DIR}/data/")
add_compile_definitions(RESULT_PATH="${PROJECT_SOURCE_DIR}/result/")
add_compile_definitions(REPORT_PATH="${PROJECT_SOURCE_DIR}/reports/")
add_compile_definitions(OBJ_PATH="${PROJECT_BINARY_DIR}/CMakeFiles/${PROJECT_NAME}.dir/")
# object file: <testfile_source.cpp>.o


# feature macros to control some libraries
# use strict JSON mode to make them more conformant

# add_compile_definitions(ARDUINOJSON_ENABLE_COMMENTS=0)
add_compile_definitions(JSMN_STRICT=1)

find_package(Boost)
if( Boost_FOUND )
    add_compile_definitions(HAS_BOOST)
endif()

find_package(Qt5 COMPONENTS Core )
if( Qt5_FOUND )
    add_compile_definitions(HAS_QT)
    include_directories ("/usr/include/qt5")

endif()

find_package(Poco REQUIRED COMPONENTS Foundation JSON)
if( Poco_FOUND )
    add_compile_definitions(HAS_POCO)
endif()

add_compile_definitions(USE_MEMORYSTAT=1)
add_compile_definitions(TEST_PARSE=1)
add_compile_definitions(TEST_STRINGIFY=1)
add_compile_definitions(TEST_PRETTIFY=1)
add_compile_definitions(TEST_STATISTICS=1)
add_compile_definitions(TEST_SAXROUNDTRIP=1)
add_compile_definitions(TEST_SAXSTATISTICS=1)
add_compile_definitions(TEST_SAXSTATISTICSUTF16=0)
add_compile_definitions(TEST_CONFORMANCE=1)
add_compile_definitions(TEST_INFO=1)

# disable old stuff of jsoncons
add_compile_definitions(JSONCONS_NO_DEPRECATED)

include_directories (
#    src/tunnuz
    externals
    thirdparty
    thirdparty/boostjson/include
    thirdparty/fastjson/include
    thirdparty/jsonbox/include
    thirdparty/jsoncpp/include
    thirdparty/rapidjson/include
    thirdparty/udp-json-parser
    thirdparty/include
    thirdparty/json-voorhees/include
    thirdparty/json-voorhees/src
    thirdparty/jsoncons/src
    thirdparty/jsoncons/include
    thirdparty/jsoncpp/include
    thirdparty/nlohmann/include
    thirdparty/ArduinoJson/src
    thirdparty/ArduinoJson/
    thirdparty/include/jeayeson/include/dummy
    thirdparty/jvar/include
    thirdparty/pjson/inc
    thirdparty/ULib/include
    thirdparty/facil.io/lib/facil
    thirdparty/facil.io/lib/facil/fiobj/fiobj
    thirdparty/facil.io/lib/facil/fiobj
    thirdparty/simdjson/singleheader
    thirdparty/yajl/src
    thirdparty/centijson/src
    thirdparty/cpprestsdk/Release/include
    thirdparty/cpprestsdk/Release/src

    # activate PEGTL -> TODO put to pull script
    # cd thirdparty/taocppjson
    # git submodule init
    # git submodule update
    thirdparty/taocppjson/external/PEGTL/include
)

file( GLOB HEADERS ${PROJECT_SOURCE_DIR}
    externals/fmt/*.h
    src/*.h
    src/cjsonlibs/*.h
)

file( GLOB SOURCES_MAIN ${PROJECT_SOURCE_DIR}
    src/*.cpp
    externals/fmt/*.cc
)

set( SOURCES_TESTS_C ${PROJECT_SOURCE_DIR}
#    src/cjsonlibs/facil.io_all.c #??
#    src/cjsonlibs/jansson_all.c
#    src/cjsonlibs/jansson_renaming.h
#    src/cjsonlibs/jsmn_all.c
#    src/cjsonlibs/parson_all.c
#    src/cjsonlibs/parson_renaming.h
#    src/cjsonlibs/pjson_all.c
#    src/cjsonlibs/sheredom_jsonh_all.c
#    src/cjsonlibs/sheredom_jsonh_renaming.h
#    src/cjsonlibs/yajl_all.c # ?
    src/cjsonlibs/jsonc_all.c
    src/cjsonlibs/cjson_all.c
    src/cjsonlibs/centijson_all.c
    src/cjsonlibs/yyjson_all.c
    src/cjsonlibs/ccan_all.c
    src/cjsonlibs/juson_all.c
    src/cjsonlibs/udpjson_all.c
    src/cjsonlibs/vincenthz_libjson_all.c
    src/cjsonlibs/vincenthz_libjson_renaming.h
)

set( SOURCES_TESTS
# add new modules
# ujson/
# ultrajson/
    src/tests/boostjsontest.cpp
    src/tests/cjsontest.cpp
    src/tests/voorheestest.cpp
    src/tests/sajsontest.cpp
    src/tests/centijsontest.cpp
    src/tests/yyjsontest.cpp
    src/tests/sheredomjsonhtest.cpp
    src/tests/vincenthzlibjsontest.cpp
    src/tests/strduptest.cpp
    src/tests/ccantest.cpp
    src/tests/arduinojsontest.cpp
    src/tests/nbsdxsimplejsontest.cpp
    src/tests/qttest.cpp
    src/tests/configurutest.cpp
    src/tests/cpprestsdktest.cpp
    src/tests/dropboxjson11test.cpp
    src/tests/fastjsontest.cpp
    src/tests/gasontest.cpp
    src/tests/jeayesontest.cpp
    src/tests/jsonboxtest.cpp
    src/tests/jsonconstest.cpp
    src/tests/jsoncpptest.cpp
    src/tests/jsonspirittest.cpp        # use thirdparty/boostspirit
    src/tests/jsonxxtest.cpp
    src/tests/jusontest.cpp
    src/tests/nlohmanntest.cpp
    src/tests/picojsontest.cpp
    src/tests/pocotest.cpp
    src/tests/rapidjsonautoutftest.cpp
    src/tests/rapidjsonfullprectest.cpp
    src/tests/rapidjsoninsitutest.cpp
    src/tests/rapidjsoniterativetest.cpp
    src/tests/rapidjsontest.cpp
    src/tests/simdjsontest.cpp
    src/tests/simplejsontest.cpp
    src/tests/taocpptest.cpp
    src/tests/udpjsontest.cpp
#    src/tests/jsmntest.cpp # tokenizer
#    src/tests/cajuntest.cpp # ok but very slow
#    src/tests/jsonctest.cpp
#    src/tests/facil.io.cpp # active - master failing - try branch 0.8 or wait for update
#    src/tests/janssontest.cpp # active - config needed
#    src/tests/parsontest.cpp # active - malloc,free
#    src/tests/ULibtest.cpp # active - ./configure is needed
#    src/tests/qajson4ctest.cpp # ?
#    src/tests/jzontest.cpp # inactive - 5y
#    src/tests/pjsontest.cpp # inactive - 4y
#    src/tests/jvartest.cpp # inactive - 4y
#    src/tests/tunnuztest.cpp # inactive - 5y
#    src/tests/yajltest.cpp # inactive - 6y
#    src/tests/follytest.cpp # installation
#    src/tests/v8test.cpp  # installation
#    src/tests/taocppnlohmanntest.cpp   # too many changes
)

file( GLOB_RECURSE SOURCES_3PRTY ${PROJECT_SOURCE_DIR}
    thirdparty/*.cpp
    thirdparty/*.c
    thirdparty/*.h
)

add_executable( ${PROJECT_NAME}
    ${SOURCES_TESTS_C}
    ${SOURCES_TESTS}
    ${SOURCES_MAIN}
    ${HEADERS}
)

target_compile_options(${PROJECT_NAME} PRIVATE -fPIC)

if( Poco_FOUND )
target_link_libraries( ${PROJECT_NAME} PUBLIC
    ${Poco_LIBRARIES}
)
endif()

if( Qt5_FOUND )
target_link_libraries( ${PROJECT_NAME} PUBLIC
    Qt5::Core
)
endif()
