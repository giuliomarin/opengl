project(OpenGL)
cmake_minimum_required(VERSION 2.8.7)

# additional cmake files
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Include additional scripts
include(cmake/Config.cmake)

# turn on using solution folders
set_property( GLOBAL PROPERTY USE_FOLDERS ON)

# Default install folder
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install")


# Setup
function(find_opencv)
  set(OpenCV_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../cpp-lib/opencv/darwin/lib64/lib/cmake/opencv4)
	find_package(OpenCV ${OPENCV_VERSION} REQUIRED)
  set (OpenCV_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS} PARENT_SCOPE)
  set (OpenCV_LIBS ${OpenCV_LIBS} PARENT_SCOPE)
endfunction(find_opencv)

function(link_opencv project_name)
	target_link_libraries(${project_name} ${OpenCV_LIBS})
	include_directories(${OpenCV_INCLUDE_DIRS})
endfunction(link_opencv)


# ----------------
# Apps
# ----------------

set(APPS 0_test 3_gui 4_gui_gl)
find_opencv()
FOREACH(app ${APPS})
    add_subdirectory(${app})
    set_property(TARGET ${app} PROPERTY FOLDER "apps")
ENDFOREACH(app)
