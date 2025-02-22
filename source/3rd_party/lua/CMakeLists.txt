set(CMAKE_C_STANDARD 11)

include(src/CMakeLists.txt)

foreach(FILE ${FILES})
  set(lua_source ${lua_source} src/${FILE})
endforeach()

add_library(lua_lib SHARED ${lua_source})

add_compile_definitions(lua_lib LUA_BUILD_AS_DLL LUA_LIB)

target_include_directories(lua_lib PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
