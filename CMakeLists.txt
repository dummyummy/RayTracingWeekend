cmake_minimum_required(VERSION 3.21)

project(RayTracingWeekend
    LANGUAGES CXX C
    VERSION 1.0.0
    DESCRIPTION "DX12 Renderer as a personal practice"
)

# PCH configuration
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable PCH support for the entire project
set(CMAKE_PCH_INSTANTIATE_TEMPLATES ON)

add_subdirectory(3rd)
add_subdirectory(lib)

add_executable(RayTracingWeekend ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(RayTracingWeekend PRIVATE external RayTracingWeekendLib)
target_compile_features(RayTracingWeekend PRIVATE cxx_std_17)
target_compile_definitions(RayTracingWeekend PRIVATE STB_IMAGE_WRITE_IMPLEMENTATION)

# Set PCH for the main executable
target_precompile_headers(RayTracingWeekend REUSE_FROM pch_lib)
