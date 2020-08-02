# http-cpp
http-cpp is a minimal HTTP server and request / response parser currently supporting `GET` methods only.
It's purpose - for me personally - was simply to better understand the Linux epoll API while also starting to develop in C++.
That being said, the code itself is fairly easy to understand and heavily commented. 

## Building
As with any CMake project create a `build/` directory, execute `cmake ..` inside it followed by `make -j`:
```Bash
mkdir build; cd build
cmake ..
make -j
``` 

## Demo
![](demo.gif)
