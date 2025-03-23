#include "ns_lua.h"
#include "curl/curl.h"

CURL *curl;

std::string g_serverURL;

struct binary_data
{
  static constexpr size_t initialSize = 4096;
  std::unique_ptr<char[]> data = std::make_unique<char[]>(initialSize);
  size_t allocatedBytes = initialSize;
  size_t usedBytes = 0;
};

size_t append(binary_data& binaryData, char* data, size_t size)
{
  size_t remainingBytes = binaryData.allocatedBytes - binaryData.usedBytes;
  size_t bytesToCopy = std::min(size, remainingBytes);
  ::memcpy(binaryData.data.get() + binaryData.usedBytes, data, bytesToCopy);
  binaryData.usedBytes += bytesToCopy;
  return bytesToCopy;
}

binary_data g_responseData;

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
  std::cout << std::format("data size '{}'\n", size * nmemb);
  append(g_responseData, ptr, size * nmemb);
  return size * nmemb;
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
    return 0;
}

static int saveResponse(lua_State* L)
{
  const char* filename = luaL_checkstring(L, 1);

  std::fstream file(filename, std::ios::out  | std::ios::binary);
  file.write(g_responseData.data.get(), g_responseData.usedBytes);
  file.close();

  std::cout << std::format("response file saved to '{}'\n", filename);
  return 0;
}

static int callServer(lua_State* L)
{
  std::cout << std::format("sending request to '{}'\n", g_serverURL.c_str());

  struct curl_slist *header = NULL;
  header = curl_slist_append(header, "Content-type: text/xml; charset=utf-8");
  header = curl_slist_append(header, "SOAPAction: http://localhost:62634/Service1.svc");
  header = curl_slist_append(header, "Transfer-Encoding: chunked");
  header = curl_slist_append(header, "Expect:");

  curl_easy_setopt(curl, CURLOPT_URL, g_serverURL.c_str());
  curl_easy_setopt(curl, CURLOPT_POST, 1);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "<?xml version=\"1.0\" encoding=\"UTF-8\"?><soapenv:Envelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:AddNumbers=\"http://localhost:62634/Service1.svc\"><soapenv:Header/><soapenv:Body><AddNumbers xmlns=\"http://tempuri.org/\"><number1>4</number1><number2>5</number2></AddNumbers></soapenv:Body></soapenv:Envelope>");
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
  
  lua_pushstring(luaState, "Hello, world!");
  lua_setglobal(luaState, "msg");
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
