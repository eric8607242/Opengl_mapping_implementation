Project Base
====
## Requirement
1. cmake
2. conan - package manager
```bash
pip3 install conan
```
3. download package
```bash
mkdir build && cd build
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
conan install .. --build glad -sbuild_type=Debug
```

## Configure
```bash
cd build
cmake .. 
```

## Build
```bash
cd build
cmake --build .
```

## Thirdparty
1. GLFW3
2. ImGui
