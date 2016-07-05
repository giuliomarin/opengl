set(project_name 0_camerascene)
project(${project_name})

# Inclusion folders
set(proj_path .)
include_directories(${proj_path}/include)

# Add sources to compile exe
file(GLOB_RECURSE project_src_files ${proj_path}/src/*)
file(GLOB_RECURSE project_hpp_files ${proj_path}/include/*)
add_executable(${project_name} ${project_src_files} ${project_hpp_files})

# Post build

add_custom_command(TARGET ${project_name} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                 ${CMAKE_CURRENT_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${project_name}>/shaders)

# -------------------
# Libraries
# -------------------

find_package(FreeImage REQUIRED)
find_package(glm REQUIRED)
FIND_LIBRARY(GLUT_LIBRARY GLUT)
FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
MARK_AS_ADVANCED (GLUT_LIBRARY OpenGL_LIBRARY)
include_directories(${FreeImage_INCLUDE_DIRS} ${glm_INCLUDE_DIRS})

set(LIBS ${LIBS} ${FreeImage_LIBRARIES} ${GLUT_LIBRARY} ${OpenGL_LIBRARY})
target_link_libraries(${project_name} ${LIBS})

# Log message
log_info("Included ${project_name}")
