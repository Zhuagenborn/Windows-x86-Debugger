add_library(thread)

set(HEADER_PATH ${PROJECT_SOURCE_DIR}/include)
target_include_directories(thread PUBLIC ${HEADER_PATH})

target_sources(thread
    PUBLIC
        ${HEADER_PATH}/thread.h
    PRIVATE
        thread.cpp
        thread.breakpoint.cpp
        thread.step.cpp
)

target_link_libraries(thread PUBLIC breakpoint)
target_link_libraries(thread PRIVATE register)
target_link_libraries(thread PRIVATE error)