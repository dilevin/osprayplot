# osprayplot
This project is a MATLAB Interface to [Intel's Ospray Visualization Renderer](https://www.ospray.org). 

ospray_sphere.m shows an example of setting up the renderer to produce an image.

![Plane](./images/sunset_plane.jpg)

*An aluminum plane at sunset, rendered from MATLAB, using Ospray*

## Installation
So far this has only been tested on OSX using Ospray intalled via Homebrew.

**WARNING:** Requires CMake version **3.17** or higher.

### OSX

## Option 1 (Easy): No Intel Deep Learning Denoiser

  1. Install Ospray using Homebrew: `brew install ospray'
  
  2. Use CMake to create build files. While in ${OSPRAYPLOT_DIR}

  ```
     mkdir ./build
     cd ./build
     cmake .. -DCMAKE_BUILD_TYPE=Release -DOspray_DIR=${Ospray_CMake_Directory} -DEmbree_DIR=${Embree_CMake_Directory} -DOspcommon_DIR=${Ospcommon_CMake_Directory} -DOpenvkl_DIR=${Openvkl_CMake_Directory}
  ```
  **NOTE:** If on OSX, when installed with *Homebrew*, Ospray, Embree, OspCommon and OpenVKL will be located in the       `/usr/local/opt/ospray/lib/cmake` directory. Your command line will look something like this: 
  ```
    cmake .. -DCMAKE_BUILD_TYPE=Release -DOspray_DIR=/usr/local/opt/ospray/lib/cmake/ospray-2.1.0 -DEmbree_DIR=/usr/local/opt/embree/lib/cmake/embree-3.9.0 -DOspcommon_DIR=/usr/local/opt/ospray/lib/cmake/ospcommon-1.3.0 -DOpenvkl_DIR=/usr/local/opt/ospray/lib/cmake/openvkl-0.9.0
  ```
    
  3. `make all`

## Option 2 (Harder): With Intel Deep Learning Denoiser 

  1. Install Git Large File Storage: `brew install git-lfs'
  
  2. Install Intel Implicit SPMD Program Compiler: `brew install ispc'
  
  3. Install Intel Embree: `brew install embree'
  
  4. Install Intel TBB: `brew install tbb'

  5. Install GLFW: `brew install glfw'
  
  6. Install rkcommon
  
   ```
    cd ${OSPRAYPLOT_DIR}/extern/rkcommon
    mkdir ./build
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_NAME_DIR=/usr/local/lib -DCMAKE_INSTALL_RPATH=/usr/local/lib
    make all
    sudo make install
   ```
    

  
## Finalize MATLAB Setup

  4. Add paths in matlab **MATLAB**:
  ```
    addpath '${OSPRAYPLOT_DIR}'
    addpath '${OSPRAYPLOT_DIR}/build'
  ```
