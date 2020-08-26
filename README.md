# NinGine

## Getting started:
### Windows:
0) Download\Install Visual Studio 19
1) Download\Install CMake
2) Download\Install vcpkg -> https://github.com/microsoft/vcpkg

	2.1) (Not required but very handy) Add vcpkg folder to PATH environment variable
3) Open project with the Visual Studio and configure CMake toolchain file:
	
	a) Open the CMake Settings Editor
	b) Under CMake toolchain file, add the path to the vcpkg toolchain file:
		[vcpkg root]/scripts/buildsystems/vcpkg.cmake
4) Install required libs with vcpkg
	
	vcpkg install glfw:x64-windows
	vcpkg install glad:x64-windows