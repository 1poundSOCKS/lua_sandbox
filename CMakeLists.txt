cmake_minimum_required(VERSION 3.25)

project(lua_sandbox_project)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(source/3rd_party/lua)

set(CMAKE_CXX_STANDARD 20)

include(source/CMakeLists.txt)
foreach(FILE ${FILES})
  set(source ${source} source/${FILE})
endforeach()

add_executable(lua_sandbox ${source} ${lua_source})

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(lua_sandbox PRIVATE /MP /MTd /Zi /W0 /WX /fp:fast)
  target_link_options(lua_sandbox PRIVATE /SUBSYSTEM:CONSOLE)
endif()

target_precompile_headers(lua_sandbox PRIVATE <iostream>)

add_compile_definitions(lua_sandbox LUA_BUILD_AS_DLL)

target_include_directories(lua_sandbox PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/source
  ${CMAKE_CURRENT_SOURCE_DIR}/source/3rd_party/lua/include
)

target_link_libraries(lua_sandbox ${CMAKE_CURRENT_SOURCE_DIR}/source/3rd_party/lua/Debug/lua_lib.lib)
