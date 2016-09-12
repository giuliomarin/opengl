set(project_name 1_camerasimulator)
project(${project_name})

# Inclusion folders
set(proj_path .)
include_directories(${proj_path}/include)

# Add sources to compile exe
file(GLOB_RECURSE project_src_files ${proj_path}/src/*.cpp)
file(GLOB_RECURSE project_hpp_files ${proj_path}/include/*.hpp)
add_executable(${project_name} ${project_src_files} ${project_hpp_files})

# -------------------
# Libraries
# -------------------

find_package(glfw REQUIRED)

FIND_LIBRARY(Cocoa Cocoa)
FIND_LIBRARY(OpenGL OpenGL)
FIND_LIBRARY(IOKit IOKit)
FIND_LIBRARY(CoreVideo CoreVideo)
MARK_AS_ADVANCED(Cocoa OpenGL IOKit CoreVideo)
include_directories(${glfw_INCLUDE_DIRS})

set(LIBS ${glfw_LIBRARY} ${Cocoa} ${OpenGL} ${IOKit} ${CoreVideo})
target_link_libraries(${project_name} ${LIBS})

# Log message
log_info("Included ${project_name}")
