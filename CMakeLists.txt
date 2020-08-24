cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Games_Engineering)

SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")

SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

add_subdirectory("lib/sfml")
set(SFML_INCS "lib/sfml/include")
link_directories("${CMAKE_BINARY_DIR}/lib/SFML/lib")




add_library(lib_maths INTERFACE)
target_sources(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib_maths/maths.h")
target_include_directories(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib_maths" SYSTEM INTERFACE ${SFML_INCS})

file(GLOB_RECURSE SOURCE_FILES lib_tile_level_loader/*.cpp lib_tile_level_loader/*.h)
add_library(lib_tile_level_loader STATIC ${SOURCE_FILES})
target_include_directories(lib_tile_level_loader INTERFACE "${CMAKE_SOURCE_DIR}/lib_tile_level_loader/" ${SFML_INCS})
target_link_libraries(lib_tile_level_loader lib_maths sfml-graphics)

file(GLOB_RECURSE SOURCES Game_Base/*.cpp Game_Base/*.h)
add_executable(Game_Base ${SOURCES})
target_include_directories(Game_Base PRIVATE ${SFML_INCS})
target_link_libraries(Game_Base lib_tile_level_loader sfml-graphics)

set_target_properties(Game_Base PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration))

add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND}
	-E copy_directory
	"${PROJECT_SOURCE_DIR}/res"
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/res)

add_dependencies(Game_Base copy_resources)


