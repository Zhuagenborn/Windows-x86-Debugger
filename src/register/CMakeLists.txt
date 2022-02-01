add_library(register)

set(HEADER_PATH ${PROJECT_SOURCE_DIR}/include/register)
target_include_directories(register PRIVATE ${HEADER_PATH})
target_include_directories(register PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_sources(register
    PUBLIC
        ${HEADER_PATH}/register.h
        ${HEADER_PATH}/registers.h
    PRIVATE
        register.cpp
        flag_register.cpp
        debug_status_register.cpp
        debug_control_register.cpp
        registers.cpp
)

target_link_libraries(register PRIVATE error)