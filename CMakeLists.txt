cmake_minimum_required(VERSION 3.25)

project(lua_sandbox_project)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(LUA_INCLUDE $ENV{LUA_INCLUDE})
set(LUA_LIB $ENV{LUA_LIB})
set(CURL_HOME $ENV{CURL_HOME})

set(CMAKE_CXX_STANDARD 23)

set(source
  source/main.cpp
  source/ns_lua.cpp
)

add_executable(lua_sandbox 
  ${source}
  C:/Users/mathe/source/repos/vcpkg/packages/lua_x64-windows/bin/lua.dll
  C:/Users/mathe/source/repos/vcpkg/packages/curl_x64-windows/bin/libcurl.dll
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  add_compile_definitions(lua_sandbox LUA_BUILD_AS_DLL)
  target_compile_options(lua_sandbox PRIVATE /MP /MTd /Zi /W0 /WX /fp:fast /std:c++latest /EHsc)
  target_link_options(lua_sandbox PRIVATE /SUBSYSTEM:CONSOLE)
else()
  add_compile_definitions(lua_sandbox LUA_USE_LINUX)
endif()

target_precompile_headers(lua_sandbox PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source/pch.h)

target_include_directories(lua_sandbox PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/source
  ${LUA_INCLUDE}
  ${CURL_HOME}/include
)

target_link_libraries(lua_sandbox ${LUA_LIB}/lua.lib)
