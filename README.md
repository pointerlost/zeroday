# zeroday Engine - Cross-Platform Game Engine

(The project isn't finished yet!)
You can review it and help, but the readme won't be updated until we're sure this version works!

## Quick Start

### Linux/Mac
```bash
git clone https://github.com/pointerlost/zeroday.git
cd zeroday
./vcpkg/bootstrap-vcpkg.sh
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release

git clone https://github.com/pointerlost/zeroday.git
cd zeroday
.\vcpkg\bootstrap-vcpkg.bat
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
