# MANZIL 

## DESCRIPTION 

Manzil, Unofficial Open Source Quranite Reader.

Quran reader for PC (Linux and Windows), made with wxWidgets and C++ 

based on : Official Quranite Reader by Sam Gerrans

## USAGE

WARNING : Make sure zig is installed and on path, zig used as scripting to generate header to embed json also as drop in cpp compiler

### Debug mode 

``` sh
git clone --recurse-submodules --shallow-submodules https://codeberg.org/fauzim/manzil.git
cd manzil
cmake -G ninja -B build 
ninja -C build 
# executable is on build/manzil
``` 
### Release mode 

Make sure zig is installed and on path, zig used for easy cross compile

``` sh
git clone --recurse-submodules --shallow-submodules https://codeberg.org/fauzim/manzil.git
cd manzil
# x86_64 Linux
# check for your glib version :
ldd --version | head -1

# change DZIG_TARGET to your current glib version 
# for example for glib version 2.43 is : x86_64-linux-gnu.2.43 
cmake -S . -B build-linux \
        -DCMAKE_TOOLCHAIN_FILE=cmake/zig-toolchain.cmake \
        -DZIG_TARGET=x86_64-linux-gnu.2.43 \
        -G Ninja

ninja -C build-linux 
# executable is on build-linux/manzil
# WARNING : assets folder must be at same directory as this executable

# x86_64 Windows GNU 
cmake -S . -B build-win \
      -DCMAKE_TOOLCHAIN_FILE=cmake/zig-toolchain.cmake \
      -DZIG_TARGET=x86_64-windows-gnu \
      -G Ninja
ninja -C build-win 
# executable is on build-win/manzil
# WARNING : assets folder must be at same directory as this executable
``` 

### Docs 
Docs currently not ready yet until v1.0.0
``` sh
cd manzil
doxygen Doxyfile
```
## LICENSE 
[License](./LICENSE)
