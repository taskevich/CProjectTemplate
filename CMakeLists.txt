cmake_minimum_required(VERSION 3.15)
project(MyApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(DEFINED PLATFORM)
    string(TOLOWER "${PLATFORM}" PLATFORM_LOWER)
else()
    set(PLATFORM_LOWER "")
endif()

if(PLATFORM_LOWER STREQUAL "windows")
    set(PLATFORM_DIR "windows")
elseif(PLATFORM_LOWER STREQUAL "unix")
    set(PLATFORM_DIR "unix")
elseif(WIN32)
    set(PLATFORM_DIR "windows")
elseif(UNIX)
    set(PLATFORM_DIR "unix")
else()
    message(FATAL_ERROR "Не удалось определить платформу. Укажите -DPLATFORM=windows или -DPLATFORM=unix")
endif()

message(STATUS "Selected platform sources: src/${PLATFORM_DIR}")

file(GLOB_RECURSE COMMON_SOURCES "${CMAKE_SOURCE_DIR}/src/common/*.cpp" "${CMAKE_SOURCE_DIR}/src/common/*.c")

file(GLOB_RECURSE PLATFORM_SOURCES
        "${CMAKE_SOURCE_DIR}/src/${PLATFORM_DIR}/*.cpp"
        "${CMAKE_SOURCE_DIR}/src/${PLATFORM_DIR}/*.c"
)

if(NOT PLATFORM_SOURCES)
    message(FATAL_ERROR "Не найдены исходники в src/${PLATFORM_DIR}")
endif()

add_executable(${PROJECT_NAME}
        ${COMMON_SOURCES}
        ${PLATFORM_SOURCES}
        include/logger.h
        src/common/logger.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_SOURCE_DIR}/include"
        "${CMAKE_SOURCE_DIR}/src/${PLATFORM_DIR}"
)

if(PLATFORM_DIR STREQUAL "windows")
    target_compile_definitions(${PROJECT_NAME} PRIVATE "OS_WINDOWS")
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /W4 /bigobj)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32) # если нужно
elseif(PLATFORM_DIR STREQUAL "unix")
    target_compile_definitions(${PROJECT_NAME} PRIVATE "OS_UNIX")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
endif()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

message(STATUS "To configure explicitly for a platform use: cmake -S . -B build -DPLATFORM=windows")
message(STATUS "Then: cmake --build build --config Release")
