# lua_sandbox

# install & build vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg integrate install

# install lua
vcpkg install lua

# install curl & libcurl
vcpkg install curl[tool]

# create CMakeUserPresets.json

{
  "version": 6,
  "configurePresets": [
      {
        "name": "my-windows",
        "inherits": "windows",
        "environment": {
          "LUA_INCLUDE": "C:/Users/mathe/source/repos/vcpkg/packages/lua_x64-windows/include",
          "LUA_LIB": "C:/Users/mathe/source/repos/vcpkg/packages/lua_x64-windows/lib",
          "CURL_INCLUDE": "C:/Users/mathe/source/repos/vcpkg/packages/curl_x64-windows/include/curl"
        }
    },
    {
      "name": "my-linux",
      "inherits": "linux",
      "environment": {
        "LUA_INCLUDE": "",
        "LUA_LIB": "",
        "CURL_INCLUDE": ""
    }
    }
  ]
}
