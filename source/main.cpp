#include "ns_lua.h"
#include "curl/curl.h"

CURL *curl;

static int registerCallback(lua_State* L)
{
    const char* callbackName = luaL_checkstring(L, 1);
    std::cout << std::format("{}\n", callbackName);
    return 0;
}

static int loadRequest(lua_State* L)
{
    const char* filename = luaL_checkstring(L, 1);
    std::cout << std::format("loading request from '{}'\n", filename);
    return 0;
}

static int callServer(lua_State* L)
{
  const char* url = luaL_checkstring(L, 1);
  std::cout << std::format("sending request to '{}'\n", url);

  curl_easy_setopt(curl, CURLOPT_URL, url);

  CURLcode res = curl_easy_perform(curl);

  if(res != CURLE_OK)
  {
    std::cout << std::format("curl_easy_perform() failed: '{}'\n", curl_easy_strerror(res));
  }

  return 0;
}

int main(int argc, char* argv[])
{
  argc;
  argv;

  std::cout << std::format("lua_sandbox started\n");

  lua::state luaState;
  luaState.create();

  curl = curl_easy_init();
  if(!curl)
  {
    std::cout << std::format("curl_easy_init() failed\n");
    return 1;
  }
  
  lua_pushstring(luaState, "Hello, world!");
  lua_setglobal(luaState, "msg");
  luaL_openlibs(luaState);
  
  lua_register(luaState, "registerCallback", registerCallback);
  lua_register(luaState, "loadRequest", loadRequest);
  lua_register(luaState, "callServer", callServer);

  if( luaL_loadfilex(luaState, "scripts/hello.lua", nullptr) || lua_pcallk(luaState, 0, LUA_MULTRET, 0, 0, 0) )
  {
      std::cout << "Error: " << lua_tostring(luaState, -1) << "\n";
  }

  curl_easy_cleanup(curl);
  luaState.close();

  return 0;
}
