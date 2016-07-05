set(deps_path ${CMAKE_CURRENT_SOURCE_DIR}/../deps)

set(FreeImage_LIBRARIES "${deps_path}/freeimage/macos/lib64/libfreeimage.a")
set(FreeImage_LIBRARY ${FreeImage_LIBRARIES})

set(FreeImage_INCLUDE_DIRS "${deps_path}/freeimage/macos/include")
set(FreeImage_INCLUDE_DIR ${FreeImage_INCLUDE_DIRS})

set(FreeImage_FOUND True)
