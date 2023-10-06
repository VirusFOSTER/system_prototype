cmake_minimum_required(VERSION 3.14)

project(system LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost 1.82 COMPONENTS filesystem system REQUIRED)
include_directories(${Boost_DIRECTORIES})
link_libraries(${Boost_LINRARIES})

add_definitions(-DPROJECT_PATH="${PROJECT_SOURCE_DIR}")

# JSON-parser
set(JSON_IO_HEADERS
    src/common/json/headers/json_loader.hpp
    src/common/json/headers/json_object.hpp
    src/common/json/headers/json_array.hpp
    src/common/json/headers/json_output.hpp
    src/common/json/headers/json_common.hpp
    src/common/json/json.hpp)

set(JSON_IO_SOURCES
    src/common/json/sources/json_loader.cpp
    src/common/json/sources/json_object.cpp
    src/common/json/sources/json_array.cpp
    src/common/json/sources/json_output.cpp
    src/common/json/sources/json_common.cpp)

set(JSON_IO
    ${JSON_IO_HEADERS}
    ${JSON_IO_SOURCES})

source_group(src_json "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${JSON_IO})

# INTERFACES SYSTEM
set(INTERFACE_HEADERS
    src/common/interfaces/buffer_concept.hpp
    src/common/interfaces/interface_concept.hpp
    src/common/interfaces/reader_sequence.hpp
    src/common/interfaces/sequence_buffer.hpp
    src/common/interfaces/writer_sequence.hpp)

source_group(src_interfaces "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${INTERFACE_HEADERS})

# BASE CONTAINERS
set(BASE_CONTAINER_SOURCES
    src/system_traits/messages/container/messages_container.cpp
    src/system_traits/components/components_container.cpp)

source_group(src_container "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${BASE_CONTAINER_SOURCES})

# CONFIGURATION SOURCES
set(CONFIGURATION_SYSTEM_HEADERS
    src/system_traits/configuration/config_reader.hpp
    src/system_traits/configuration/configuration.hpp
    src/system_traits/configuration/cfg_type_readers/system_component_config.hpp
    src/system_traits/configuration/cfg_type_readers/system_messages_config.hpp)

set(CONFIGURATION_SYSTEM_SOURCES
    src/system_traits/configuration/config_reader.cpp
    src/system_traits/configuration/configuration.cpp
    src/system_traits/configuration/cfg_type_readers/system_component_config.cpp
    src/system_traits/configuration/cfg_type_readers/system_messages_config.cpp)

set(CONFIGURATION_SYSTEM
    ${CONFIGURATION_SYSTEM_HEADERS}
    ${CONFIGURATION_SYSTEM_SOURCES})

source_group(src_config "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${CONFIGURATION_SYSTEM_SOURCES})

# FUNCTIONAL COMPONENTS SYSTEM
set(BASE_COMPONENT_SOURCES
    src/system_traits/components/base_fc.cpp)

source_group(src_components "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${BASE_COMPONENTS_SOURCES})

# ALL SYSTEM SOURCES
set(SYSTEM_SOURCES
    src/system_traits/system.cpp
    ${JSON_IO}
    ${CONFIGURATION_SYSTEM}
    ${INTERFACE_HEADERS}
    ${BASE_CONTAINER_SOURCES}
    ${BASE_COMPONENT_SOURCES})

add_executable(system
  main.cpp
  ${SYSTEM_SOURCES}
)

target_link_libraries(system
    ${Boost_LIBRARIES}
    pthread
    boost_thread)
