file(GLOB_RECURSE module_factory_SOURCES *.cpp *.h)
add_executable(module_factory ${module_factory_SOURCES})

find_library(libswf swf)
find_library(libz z)
target_link_libraries(module_factory ${libswf} ${libz})
