cmake_minimum_required(VERSION 3.8.2)

#project name and cpp standard (c++11)
project("CMake Test" CXX)
set(CMAKE_CXX_STANDARD 11)
add_compile_options(-g)
set(app_name "genericSDL")

#include_directories(src)#will search inside all directories inside src
add_subdirectory(src)


#Get include directories from sub folders
set(INCLUDE_DIRECTORIES "")
get_directory_property(INCLUDE_DIRECTORIES DIRECTORY src DEFINITION INCLUDE_DIRECTORIES)
#Add include directories
include_directories(${INCLUDE_DIRECTORIES})

#Get Source files paths from subfolders
set(SOURCE_FILES "")
get_directory_property(SOURCE_FILES DIRECTORY src DEFINITION SOURCE_FILES)

#Get Header files paths
set(HEADER_FILES "")
get_directory_property(HEADER_FILES DIRECTORY src DEFINITION HEADER_FILES)


# includes cmake_modules/FindSDL2*.cmake
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
#SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
#SDL2 image
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
#SDL2 ttf
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_TTF_INCLUDE_DIR})
#SDL2 mixer
find_package(SDL2_mixer REQUIRED)
include_directories(${SDL2_MIXER_INCLUDE_DIR})
#SDL2 net
#find_package(SDL2_net REQUIRED)
#include_directories(${SDL2_NET_INCLUDE_DIR})

add_executable(${app_name} ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(${app_name} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY} ${SDL2_MIXER_LIBRARY})

#copy contents of root folder "data" to folder "data" next to executable, if build is succeeds(POST_BUILD)
add_custom_command(TARGET ${app_name} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:${app_name}>/data)
