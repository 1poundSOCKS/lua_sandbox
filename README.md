# lua_sandbox

# install & build vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg integrate install

# install curl & libcurl
vcpkg install curl[tool]

# setup env vars

e.g. for Windows 64 bit
CURL_HOME=<vcpkg_location>\vcpkg\packages\curl_x64-windows
