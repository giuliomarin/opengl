set(project_name 0_test)
project(${project_name})

# Inclusion folders
set(proj_path "${CMAKE_CURRENT_SOURCE_DIR}")
include_directories(${proj_path}/include)

# Add sources to compile exe
file(GLOB_RECURSE src_files ${proj_path}/src/*.cpp)
file(GLOB_RECURSE hpp_files ${proj_path}/include/*.hpp)
add_executable(${project_name} ${src_files} ${hpp_files})
# -------------------
# Libraries
# -------------------

include_linmath()
link_glfw(${project_name})

# Log message
log_info("Included ${project_name}")
