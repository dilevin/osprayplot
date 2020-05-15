# osprayplot
This project is a MATLAB Interface to [Intel's Ospray Visualization Renderer](https://www.ospray.org). 

ospray_sphere.m shows an example of setting up the renderer to produce an image.

![Plane](./images/sunset_plane.jpg)

*An aluminum plane at sunset, rendered from MATLAB, using Ospray*

## Installation
So far this has only been tested on OSX using Ospray intalled via Homebrew.

### OSX
  1. Install Ospray using Homebrew: `brew install ospray`
  
  2. Use CMake to create build files. While in ${OSPRAYPLOT_DIR}

  ```
     mkdir ./build
     cd ./build
     cmake .. -DCMAKE_BUILD_TYPE=Release -DOspray_DIR= ${Ospray_CMake_Directory} ...
              -DEmbree_DIR=${Embree_CMake_Directory} -DOspcommon_DIR=${Ospcommon_CMake_Directory}...
              -DOpenvkl_DIR=${Openvkl_CMake_Directory}
  ```
    
  3. `make all`
  4. Add paths in matlab **MATLAB**:
  ```
    a. addpath '${OSPRAYPLOT_DIR}'
    b. addpath '${OSPRAYPLOT_DIR}/build'
  ```
