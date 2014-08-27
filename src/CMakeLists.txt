file(GLOB_RECURSE module_demo_SOURCES *.cpp *.h)
add_executable(module_demo ${module_demo_SOURCES})

find_library(libswf swf)
find_library(libz z)
target_link_libraries(module_demo ${libswf} ${libz})
