# lua_sandbox

# install & build vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg integrate install

# install curl & libcurl
vcpkg install curl[tool]

# setup env vars

CURL_HOME=C:\Users\mathe\source\repos\vcpkg\packages\curl_x64-windows