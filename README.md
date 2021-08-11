[![Build Status](https://ci.appveyor.com/api/projects/status/lh6a93nw50rxq3f7/branch/master?svg=true)](https://ci.appveyor.com/project/malynovskiy/ningine)
# NinGine

## About:
Ningine - is the game engine project, created and developed in educational purposes. It's going to be the part of my diploma work. 

![Glass](demo_glass.gif)
![Reflection](demo_reflecton.gif)

## Getting started:
### Windows:

0) Download & Install Visual Studio 19
1) Download & Install CMake
2) Install *vcpkg*
```cmd
> git clone https://github.com/microsoft/vcpkg
> ./vcpkg/bootstrap-vcpkg.sh
```

	2.1) (Not required but very handy) Add vcpkg folder to PATH environment variable
3) Open the project with Visual Studio and configure CMake toolchain file:
	
	a) Open CMake Settings Editor
	
	b) Under CMake 	toolchain file, add the path to the vcpkg toolchain file:
		[vcpkg root]/scripts/buildsystems/vcpkg.cmake
4) Install all required packadges with vcpkg
	
```cmd
> vcpkg install glfw:x64-windows
> vcpkg install glad:x64-windows
> vcpkg install glm:x64-windows
> vcpkg install opencl:x64-windows
```

### Linux:

1) Install CMake
2) Install *vcpkg*
```bash
$ git clone https://github.com/microsoft/vcpkg
$ ./vcpkg/bootstrap-vcpkg.sh
```
3) Install all required packadges with vcpkg
```bash
$ ./[vcpkg root]/vcpkg install glfw3:x64-linux
$ ./[vcpkg root]/vcpkg install glad:x64-linux
$ ./[vcpkg root]/vcpkg install glm:x64-linux
$ ./[vcpkg root]/vcpkg install opencl:x64-linux
```

### MacOS:

0) Install *Homebrew* package manager
```sh
$ /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"
```
1) Install *gcc* via *brew*
```sh
$ brew install gcc
```
2) Install *vcpkg*
```sh
$ git clone https://github.com/microsoft/vcpkg
$ ./vcpkg/bootstrap-vcpkg.sh
```
3) Install all required packadges with vcpkg
```sh
$ sudo ./[vcpkg root]/vcpkg install glfw:x64-osx
$ sudo ./[vcpkg root]/vcpkg install glad:x64-osx
$ sudo ./[vcpkg root]/vcpkg install glm:x64-osx
$ sudo ./[vcpkg root]/vcpkg install opencl:x64-osx
```

## BUILD:

### Windows:

Open a project with Visual Studio 2019 and build

### Linux:

Since we are using Cmake + vcpkg we should pass toolchain path for a build
```bash
$ mkdir [path to NinGine]/build
$ cmake -B [path to NinGine]/build -S [path to NinGine] -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
$ cmake --build [path to NinGine]/build
```
### MacOS:

- WIP
