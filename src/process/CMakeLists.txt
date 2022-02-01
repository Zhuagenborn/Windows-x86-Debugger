add_library(process)

set(HEADER_PATH ${PROJECT_SOURCE_DIR}/include)
target_include_directories(process PUBLIC ${HEADER_PATH})

target_sources(thread
    PUBLIC
        ${HEADER_PATH}/process.h
    PRIVATE
        process.cpp
        process.memory.cpp
        process.thread.cpp
        process.hardware_breakpoint.cpp
        process.software_breakpoint.cpp
)

target_link_libraries(process PUBLIC breakpoint)
target_link_libraries(process PUBLIC thread)
target_link_libraries(process PRIVATE error)