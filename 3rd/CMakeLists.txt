set(SPDLOG_BUILD_TESTS OFF CACHE INTERNAL "")
set(SPDLOG_BUILD_EXAMPLE OFF CACHE INTERNAL "")

set(INDICATORS_BUILD_TESTS OFF CACHE INTERNAL "Build indicators tests + enable CTest")
set(INDICATORS_SAMPLES OFF CACHE INTERNAL "Build indicators samples")
set(INDICATORS_DEMO OFF CACHE INTERNAL "Build indicators demo")

# Enable PCH support
set(CMAKE_PCH_INSTANTIATE_TEMPLATES ON)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/spdlog)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/stb)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/CLI11)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/indicators)

add_library(pch_lib STATIC pch.cpp)
target_include_directories(pch_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(pch_lib PUBLIC
    spdlog::spdlog
    stb_image $<$<BOOL:${MINGW}>:ws2_32>
    CLI11
    indicators
)
target_precompile_headers(pch_lib PUBLIC pch.h)

add_library(external INTERFACE)
target_link_libraries(external INTERFACE pch_lib)