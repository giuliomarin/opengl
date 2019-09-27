set(project_name 0_test)
project(${project_name})

# Inclusion folders
set(proj_path "${CMAKE_CURRENT_SOURCE_DIR}")
set(deps_path "${proj_path}/../deps")
log_info(${deps_path})
include_directories(${proj_path}/include ${CMAKE_CURRENT_SOURCE_DIR}/../../cpp-lib ${CMAKE_CURRENT_SOURCE_DIR}/../../cpp-lib/linmath)

# Add sources to compile exe
file(GLOB_RECURSE project_src_files ${proj_path}/src/*.cpp)
file(GLOB_RECURSE project_hpp_files ${proj_path}/include/*.hpp)
set(glad_src ${CMAKE_CURRENT_SOURCE_DIR}/../../cpp-lib/glad/glad_gl.c ${CMAKE_CURRENT_SOURCE_DIR}/../../cpp-lib/glad/glad_vulkan.c)
add_executable(${project_name} ${project_src_files} ${project_hpp_files} ${glad_src})

# -------------------
# Libraries
# -------------------

set(glfw3_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../cpp-lib/glfw/darwin/lib64/lib/cmake/glfw3")
find_package(glfw3 3.3 REQUIRED)

set(LIBS ${LIBS} glfw)
target_link_libraries(${project_name} ${LIBS})

# Log message
log_info("Included ${project_name}")
