#include "pch.h"
#include "ns_lua.h"
#include "curl/curl.h"
#include "binary_data.h"

CURL *curl;

std::string g_serverURL;

binary_data_owner g_requestData;
binary_data_owner g_responseData;

binary_data_writer g_requestWriter { g_requestData.size, g_requestData.data.get(), 0 };
binary_data_writer g_responseWriter { g_responseData.size, g_responseData.data.get(), 0 };

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
  size_t dataSize = size * nmemb;
  std::cout << std::format("data size '{}'\n", dataSize);
  append(g_responseWriter, binary_data { dataSize, ptr });
  return dataSize;
}

static int setServerURL(lua_State* L)
{
  const char* filename = luaL_checkstring(L, 1);
  g_serverURL = filename;
  return 0;
}

static int loadRequest(lua_State* L)
{
    const char* filename = luaL_checkstring(L, 1);
    std::cout << std::format("loading request from '{}'\n", filename);    

    std::fstream file(filename, std::ios::in  | std::ios::binary);
    g_requestWriter.position = 0;
    size_t bytesRead = read(g_requestWriter, file);
    std::cout << std::format("{} bytes read from file\n", bytesRead);
    return 0;
}

static int saveResponse(lua_State* L)
{
  const char* filename = luaL_checkstring(L, 1);
  std::cout << std::format("saving response data to '{}'\n", filename);

  std::fstream file(filename, std::ios::out  | std::ios::binary);
  size_t bytesWritten = write(file, binary_data { g_responseWriter.position , g_responseData.data.get() });
  std::cout << std::format("{} bytes written to file\n", bytesWritten);
  file.close();

  return 0;
}

static int callServer(lua_State* L)
{
  std::cout << std::format("sending request to '{}'\n", g_serverURL.c_str());

  g_requestWriter.data[g_requestWriter.position] = '\0';
  const char* requestString = static_cast<const char*>(g_requestWriter.data);

  struct curl_slist *header = NULL;
  header = curl_slist_append(header, "Content-type: text/xml; charset=utf-8");
  header = curl_slist_append(header, "SOAPAction: http://localhost:62634/Service1.svc");
  header = curl_slist_append(header, "Transfer-Encoding: chunked");
  header = curl_slist_append(header, "Expect:");

  curl_easy_setopt(curl, CURLOPT_URL, g_serverURL.c_str());
  curl_easy_setopt(curl, CURLOPT_POST, 1);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestString);
  curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)-1);
  
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
  
  lua_pushstring(luaState, "lua_sandbox");
  lua_setglobal(luaState, "host_name");
  luaL_openlibs(luaState);
  
  lua_register(luaState, "setServerURL", setServerURL);
  lua_register(luaState, "loadRequest", loadRequest);
  lua_register(luaState, "saveResponse", saveResponse);
  lua_register(luaState, "callServer", callServer);

  if( luaL_loadfilex(luaState, "scripts/hello.lua", nullptr) || lua_pcallk(luaState, 0, LUA_MULTRET, 0, 0, 0) )
  {
      std::cout << "Error: " << lua_tostring(luaState, -1) << "\n";
  }

  curl_easy_cleanup(curl);
  luaState.close();

  return 0;
}
