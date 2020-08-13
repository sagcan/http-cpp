# http-cpp
http-cpp is a minimal HTTP server and request / response parser currently supporting `GET` methods only.
It's purpose - for me personally - was simply to better understand the Linux epoll API while also starting to develop in C++.
That being said, the code itself is fairly easy to understand and heavily commented. 

## Building
http-cpp depends on 2 libraries, namely 
* spdlog
* gtest

On Ubuntu 20.04 we'd install both libraries like so:
```
# spdlog
sudo apt install libspdlog-dev

# gtest
git clone https://github.com/google/googletest
cd googletest

mkdir build && cd build
cmake .. -DBUILD_SHARE_LIBS=ON
make -j
sudo make install
```
Once done, http-cpp itself can be compiled like any other CMake project:
```Bash
mkdir build && cd build
cmake ..
make -j
``` 

## Demo
![](demo.gif)
