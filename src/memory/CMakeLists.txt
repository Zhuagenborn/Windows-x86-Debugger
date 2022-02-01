add_library(memory)

set(HEADER_PATH ${PROJECT_SOURCE_DIR}/include)
target_include_directories(memory PUBLIC ${HEADER_PATH})

target_sources(memory
    PUBLIC
        ${HEADER_PATH}/memory.h
    PRIVATE
        memory.cpp
)