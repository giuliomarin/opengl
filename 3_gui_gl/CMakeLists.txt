set(project_name 3_gui_gl)
project(${project_name})

# Inclusion folders
set(proj_path "${CMAKE_CURRENT_SOURCE_DIR}")
include_directories(${proj_path}/include)
include_linmath()

# Add sources to compile exe
file(GLOB_RECURSE src_files ${proj_path}/src/*.cpp)
file(GLOB_RECURSE hpp_files ${proj_path}/include/*.hpp)
add_executable(${project_name} ${src_files} ${hpp_files})

# -------------------
# Libraries
# -------------------

link_glfw(${project_name})
link_nanogui(${project_name})
link_opencv(${project_name})

# Log message
log_info("Included ${project_name}")
