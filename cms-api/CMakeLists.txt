cmake_minimum_required(VERSION 3.10)

project(cms-api VERSION 1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(pugixml CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(CURL REQUIRED)

include_directories(include ${CMAKE_SOURCE_DIR}/lib/oatpp/src)
include_directories(include ${CMAKE_SOURCE_DIR}/lib/cpp-driver/include)
include_directories(include ${CMAKE_SOURCE_DIR}/lib/minio-cpp/include)
include_directories(include ${CMAKE_SOURCE_DIR}/lib/inih/cpp)
include_directories(include ${CMAKE_SOURCE_DIR}/lib/inih)
include_directories(include ${CMAKE_SOURCE_DIR}/lib/onnxruntime/include)
include_directories(include ${CMAKE_SOURCE_DIR}/include)
include_directories(include ${CMAKE_SOURCE_DIR}/src)
include_directories(include ${CMAKE_SOURCE_DIR}/src/app)
include_directories(include ${CMAKE_SOURCE_DIR}/test)

set(SOURCES
    lib/inih/cpp/INIReader.cpp
    lib/inih/ini.c
    src/database/scylla_db_session.cpp
    src/database/scylla_db_utils.cpp
    src/storage/minio_client.cpp
    src/vectordb/qdrant_client.cpp
    src/utils/dotenv.cpp
    src/utils/strings.cpp
    src/app/collection/collection_controller.cpp
    src/app/collection/collection_service.cpp
    src/app/entry/entry_controller.cpp
    src/app/entry/entry_service.cpp
    src/llm_engine/tokenizer.cpp
    src/llm_engine/onnx.cpp
    test/qdrant_client_test.cpp
    test/json_test.cpp
    src/main.cpp
)

add_executable(cms-api ${SOURCES})

link_directories(${CMAKE_SOURCE_DIR}/lib/cpp-driver/build)

target_link_libraries(cms-api ${CMAKE_SOURCE_DIR}/lib/oatpp/build/src/liboatpp.a)
target_link_libraries(cms-api ${CMAKE_SOURCE_DIR}/lib/cpp-driver/build/libscylla-cpp-driver.so)
target_link_libraries(cms-api ${CMAKE_SOURCE_DIR}/lib/minio-cpp/build/libminiocpp.a)
target_link_libraries(cms-api ${CMAKE_SOURCE_DIR}/lib/curlpp/build/libcurlpp.so)
target_link_libraries(cms-api ${CMAKE_SOURCE_DIR}/lib/tokenizers/libtokenizers.so)
target_link_libraries(cms-api ${CMAKE_SOURCE_DIR}/lib/onnxruntime/lib/libonnxruntime.so)
target_link_libraries(cms-api OpenSSL::SSL)
target_link_libraries(cms-api OpenSSL::Crypto)
target_link_libraries(cms-api ZLIB::ZLIB)
target_link_libraries(cms-api pugixml::pugixml)
target_link_libraries(cms-api nlohmann_json nlohmann_json::nlohmann_json)
target_link_libraries(cms-api CURL::libcurl)