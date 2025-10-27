
# source files
file(GLOB SOURCES ./*.cpp)
file(GLOB HEADERS ./*.h)
file(GLOB SHADERS ./*.vert ./*.frag)

# TODO: to native path stuff? save to file instead of commandline argument
set(TEXTURE_PATH "${CMAKE_SOURCE_DIR}/textures")
set(SHADER_PATH  "${CMAKE_SOURCE_DIR}/shaders")
add_definitions("-DTEXTURE_PATH=\"${TEXTURE_PATH}\"")
add_definitions("-DSHADER_PATH=\"${SHADER_PATH}\"")

# executable
add_executable(SolarSystem ${HEADERS} ${SOURCES})
target_include_directories(SolarSystem PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(SolarSystem PRIVATE $<$<CXX_COMPILER_ID:MSVC>:_USE_MATH_DEFINES>)
target_compile_definitions(SolarSystem PRIVATE $<$<CXX_COMPILER_ID:MSVC>:WIN32_LEAN_AND_MEAN>)
target_compile_definitions(SolarSystem PRIVATE $<$<CXX_COMPILER_ID:MSVC>:NOMINMAX>)
target_compile_options(SolarSystem PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/wd4305>) # C4305: 'argument': truncation from 'double' to 'const float'
target_compile_options(SolarSystem PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/wd4244>) # warning C4244: 'initializing': conversion from 'double' to 'float', possible loss of data
target_compile_options(SolarSystem PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/wd4267>) # argument': conversion from 'size_t' to '_Ty', possible loss of data 

target_link_libraries(SolarSystem glfw lodePNG::lodePNG glew::glew OpenGL::GL)
