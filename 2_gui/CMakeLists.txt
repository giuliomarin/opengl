set(project_name 2_gui)
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

link_glfw(${project_name})
link_nanogui(${project_name})

# Log message
log_info("Included ${project_name}")
