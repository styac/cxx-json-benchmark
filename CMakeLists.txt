cmake_minimum_required(VERSION 3.17)

project(nativejson-benchmark CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(BUILD_SHARED_LIBS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fPIE -march=native -O2 -Wall" )
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.2 -mavx -O3 -Wall" )

set( CMAKE_BUILD_TYPE Release )
add_compile_definitions(TEST_DATA_FOLDER=${PROJECT_SOURCE_DIR}/data)
add_compile_definitions(RESULT_FOLDER=${PROJECT_SOURCE_DIR}/result)

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

add_compile_definitions( USE_MEMORYSTAT=1)
add_compile_definitions( TEST_PARSE=1)
add_compile_definitions( TEST_STRINGIFY=1)
add_compile_definitions( TEST_PRETTIFY=1)
add_compile_definitions( TEST_TEST_STATISTICS=1)
add_compile_definitions( TEST_SAXROUNDTRIP=0)
add_compile_definitions( TEST_SAXSTATISTICS=0)
add_compile_definitions( TEST_SAXSTATISTICSUTF16=0)
add_compile_definitions( TEST_CONFORMANCE=1)
add_compile_definitions( TEST_INFO=1)

# disable old stuff of jsoncons
add_compile_definitions(JSONCONS_NO_DEPRECATED)

include_directories (
    "src/tunnuz/",
    "thirdparty/",
    "thirdparty/fastjson/include/",
    "thirdparty/jsonbox/include/",
    "thirdparty/jsoncpp/incCMakeLists.txtlude/",
    "thirdparty/rapidjson/include/",
    "thirdparty/udp-json-parser/",
    "thirdparty/include/",
    "thirdparty/json-voorhees/include",
    "thirdparty/json-voorhees/src",
    "thirdparty/jsoncons/src",
    "thirdparty/jsoncons/include",
    "thirdparty/jsoncpp/include",
    "thirdparty/nlohmann/include",
    "thirdparty/ArduinoJson/src",
    "thirdparty/ArduinoJson/",
    "thirdparty/include/jeayeson/include/dummy",
    "thirdparty/jvar/include",
    "thirdparty/pjson/inc",
    "thirdparty/ULib/include",
    "thirdparty/facil.io/lib/facil",
    "thirdparty/facil.io/lib/facil/fiobj/fiobj",
    "thirdparty/simdjson/singleheader",
    "thirdparty/yajl/src"
    "thirdparty/ujson4c/3rdparty/"
)

file( GLOB HEADERS ${PROJECT_SOURCE_DIR}
    src/*.h
    src/cjsonlibs/*.h
)

file( GLOB SOURCES_MAIN ${PROJECT_SOURCE_DIR}
    src/*.cpp
)

set( SOURCES_TESTS_C ${PROJECT_SOURCE_DIR}
#    src/cjsonlibs/cjson_all.c
#    src/cjsonlibs/facil.io_all.c
#    src/cjsonlibs/jansson_all.c
#    src/cjsonlibs/jansson_renaming.h
#    src/cjsonlibs/jsmn_all.c
#    src/cjsonlibs/jsonc_all.c
#    src/cjsonlibs/parson_all.c
#    src/cjsonlibs/parson_renaming.h
#    src/cjsonlibs/pjson_all.c
#    src/cjsonlibs/qajson4c_all.c
#    src/cjsonlibs/sheredom_jsonh_all.c
#    src/cjsonlibs/sheredom_jsonh_renaming.h
#    src/cjsonlibs/yajl_all.c
    src/cjsonlibs/ccan_all.c
    src/cjsonlibs/juson_all.c
    src/cjsonlibs/udpjson_all.c
    src/cjsonlibs/ujson4c_all.c
    src/cjsonlibs/ultrajsondec_all.c
    src/cjsonlibs/vincenthz_libjson_all.c
    src/cjsonlibs/vincenthz_libjson_renaming.h
)

set( SOURCES_TESTS
#    src/tests/janssontest.cpp
#    src/tests/cjsontest.cpp
#    src/tests/facil.io.cpp
#    src/tests/follytest.cpp
#    src/tests/jsonctest.cpp
#    src/tests/jvartest.cpp
#    src/tests/jzontest.cpp
#    src/tests/parsontest.cpp
#    src/tests/pjsontest.cpp
#    src/tests/qajson4ctest.cpp
#    src/tests/sajsontest.cpp
#    src/tests/taocppnlohmanntest.cpp   # too many changes
#    src/tests/tunnuztest.cpp
#    src/tests/ujsontest.cpp
#    src/tests/ULibtest.cpp
#    src/tests/v8test.cpp
#    src/tests/voorheestest.cpp
#    src/tests/yajltest.cpp
    src/tests/sheredomjsonhtest.cpp
    src/tests/ujson4ctest.cpp
    src/tests/vincenthzlibjsontest.cpp # conformance memory leak
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
    src/tests/jsmntest.cpp
    src/tests/jsonboxtest.cpp
    src/tests/jsonconstest.cpp
    src/tests/jsoncpptest.cpp
    src/tests/jsonspirittest.cpp
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
    src/tests/cajuntest.cpp
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