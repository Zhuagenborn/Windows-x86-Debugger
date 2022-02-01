add_subdirectory(error)
add_subdirectory(breakpoint)
add_subdirectory(register)
add_subdirectory(thread)
add_subdirectory(memory)
add_subdirectory(process)

add_library(debugger)

set(HEADER_PATH ${PROJECT_SOURCE_DIR}/include)
target_include_directories(debugger PUBLIC ${HEADER_PATH})

target_sources(debugger
    PUBLIC
        ${HEADER_PATH}/debugger.h
    PRIVATE
        debugger.cpp
        debugger.dll.cpp
        debugger.rip.cpp
        debugger.thread.cpp
        debugger.process.cpp
        debugger.unknown.cpp
        debugger.exception.cpp
        debugger.debug_string.cpp
)

target_link_libraries(debugger PUBLIC thread)
target_link_libraries(debugger PUBLIC process)
target_link_libraries(debugger PRIVATE register)
target_link_libraries(debugger PRIVATE error)