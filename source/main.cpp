// #include "pch.h"
#include <iostream>
#include <lua.hpp>

int main(int argc, char* argv[])
{
  argc;
  argv;

  lua_State* L;
  L = luaL_newstate();
  
  lua_pushstring(L, "Anna");
  lua_setglobal(L, "name");
  luaL_openlibs(L);
  if(luaL_dofile(L, "scripts/hello.lua")) {
      std::cout << "Final:" << lua_tostring(L, -1) << "\n";
  }
  
  lua_close(L);

  return 0;
}
