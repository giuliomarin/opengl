set(deps_path ${CMAKE_CURRENT_SOURCE_DIR}/../deps)

set(glfw_LIBRARIES "${deps_path}/glfw/macos/lib/libglfw3.a")
set(glfw_LIBRARY ${glfw_LIBRARIES})

set(glfw_INCLUDE_DIRS "${deps_path}/glfw/macos/include")
set(glfw_INCLUDE_DIR ${glfw_INCLUDE_DIRS})

set(glfw_FOUND True)
