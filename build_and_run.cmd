cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cd build
cmake --build . --target fractal
cmake --build . --target morphing
cd fractal
start .\fractal.exe
cd ../morphing
start .\morphing.exe