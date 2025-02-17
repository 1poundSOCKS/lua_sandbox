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
  
  if( luaL_loadfilex(L, "scripts/hello.lua", nullptr) || lua_pcallk(L, 0, LUA_MULTRET, 0, 0, NULL) )
  {
      std::cout << "Error: " << lua_tostring(L, -1) << "\n";
  }
  
  lua_close(L);

  return 0;
}
