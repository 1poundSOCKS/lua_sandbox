#include "ns_lua.h"

int main(int argc, char* argv[])
{
  argc;
  argv;

  lua::state luaState;
  luaState.create();
  
  lua_pushstring(luaState, "Hello, world!");
  lua_setglobal(luaState, "msg");
  luaL_openlibs(luaState);
  
  if( luaL_loadfilex(luaState, "scripts/hello.lua", nullptr) || lua_pcallk(luaState, 0, LUA_MULTRET, 0, 0, 0) )
  {
      std::cout << "Error: " << lua_tostring(luaState, -1) << "\n";
  }
  
  luaState.close();

  return 0;
}
