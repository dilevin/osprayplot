cmake_minimum_required (VERSION 3.17)

project (ospray_plot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_PREFIX_PATH ${Ospray_DIR} ${Embree_DIR} ${Ospcommon_DIR} ${Openvkl_DIR})


find_package(Matlab REQUIRED)
find_package(embree 3.8 REQUIRED)
find_package(ospray 2.1 REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${Matlab_INCLUDE_DIRS})

matlab_add_mex(NAME osprayplot_mex
			SRC ${PROJECT_SOURCE_DIR}/src/osprayplot_mex.cpp ${PROJECT_SOURCE_DIR}/src/osprayplot_cpp.cpp
			LINK_TO ${Matlab_MX_LIBRARY} ${Matlab_MEX_LIBRARY} ${Matlab_OMP_LIB} ospray::ospray
            OUTPUT_NAME osprayplot_mex)
            


