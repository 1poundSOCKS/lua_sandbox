cmake_minimum_required(VERSION 3.25)

project(buck-wood)

set(CMAKE_CXX_STANDARD 20)

include(source/CMakeLists.txt)
foreach(FILE ${FILES})
  set(source ${source} source/${FILE})
endforeach()

add_executable(lua_sandbox ${source})

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(lua_sandbox PRIVATE /MP /MTd /Zi /W4 /WX /fp:fast)
  target_link_options(lua_sandbox PRIVATE /SUBSYSTEM:CONSOLE)
endif()

target_precompile_headers(lua_sandbox PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source/pch.h)

target_include_directories(lua_sandbox PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/source
)
