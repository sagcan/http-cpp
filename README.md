# http-cpp
http-cpp is a minimal HTTP server and request / response parser currently supporting `GET` methods only.
It's purpose - for me personally - was simply to better understand the Linux epoll API while also starting to develop in C++.
That being said, the code itself is fairly easy to understand and heavily commented. 

## Building
http-cpp has a spdlog dependecy, so head over to https://github.com/gabime/spdlog and follow to the instructions as to how to install it.
Other than that http-cpp can be build as with any CMake project by creating a `build/` directory, executing `cmake ..` inside it followed by a `make -j`:
```Bash
mkdir build; cd build
cmake ..
make -j
``` 

## Demo
![](demo.gif)
