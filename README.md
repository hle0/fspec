## Installation

First, install CMake and `libsdl2-dev` (or whatever it is called on your system). Then:

```sh
# Load the submodules
git submodule update --recursive
# Make a build directory
mkdir build && cd build
# Run cmake
cmake ..
# Run make
make
# Run the program
./src/fspec
```