# Ray Tracer

Implementation of ray tracing algorithm

## About

Implementation of a simple ray tracer in C++ 

* Supports loading of .obj files and textures 
* Implemented bounding boxes for faster ray-mesh intersection check
* Parallelized using OpenMP
* Implemented Anti-aliasing (SSAA) and Depth Of Field 


## How To Run This On Windows

-   Clone the repository including the submodules using :
```properties
git clone --recurse-submodules https://github.com/The-curs0r/RayTracer.git
```
-   Go into the cloned repository and run CMake :
```properties
cd RayTracer
cmake CMakeLists.txt -B ./build/ -G "Visual Studio 16 2019"
cd build
start rayTracer.sln
```
-   Build the solution for Release configuration and run the application
```properties
cd Release
start .\rayTracer.exe
```
## How To Run This On Linux

-   Clone the repository including the submodules using :
```properties
git clone --recurse-submodules https://github.com/The-curs0r/RayTracer.git
```
-   Go into the cloned repository and run CMake :
```properties
cd RayTracer
cmake CMakeLists.txt -B ./build/ -G "Unix Makefiles"
cd build
make all
./rayTracer
```
## Bug/Feature Request

If you find a bug, or are unable to build the project or run the application, kindly open an issue [here](https://github.com/The-curs0r/RayTracer/issues/new?assignees=&labels=&template=bug_report.md&title=).

  

Currently I'm not working on this project but if you have any ideas for the project, please put them [here](https://github.com/The-curs0r/RayTracer/issues/new?assignees=&labels=&template=feature_request.md&title=).

  

## To-Do

* Implement MSAA and/or Post Processing Anti Aliasing methods (like MLAA, FXAA and SMAA).

* Add more basic shapes (like Torus, Cubes).

* Improve lighting calculations (add glossy reflection and area lights).

  

## Results

* More images in SampleImages folder.

![](SampleImages/Scene_4_Spheres4k.png)

![](SampleImages/Spheres.png)

![](SampleImages/LitJupiter.png)