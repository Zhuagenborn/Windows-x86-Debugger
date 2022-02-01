add_library(error)

set(HEADER_PATH ${PROJECT_SOURCE_DIR}/include)
target_include_directories(error PUBLIC ${HEADER_PATH})

target_sources(error
    PUBLIC
        ${HEADER_PATH}/error.h
    PRIVATE
        error.cpp
)