#include <lua.hpp>

int main(int argc, char* argv[])
{
  argc;
  argv;

  lua_State* L;
  L = luaL_newstate();
  
  lua_pushstring(L, "Hello, world!");
  lua_setglobal(L, "msg");
  luaL_openlibs(L);
  
  if(luaL_dofile(L, "scripts/hello.lua")) {
      std::cout << "Final:" << lua_tostring(L, -1) << "\n";
  }
  
  lua_close(L);

  return 0;
}
