add_library(breakpoint)

set(HEADER_PATH ${PROJECT_SOURCE_DIR}/include)
target_include_directories(breakpoint PUBLIC ${HEADER_PATH})

target_sources(breakpoint
    PUBLIC
        ${HEADER_PATH}/breakpoint.h
    PRIVATE
        breakpoint.cpp
)