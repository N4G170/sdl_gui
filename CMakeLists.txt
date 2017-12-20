cmake_minimum_required(VERSION 3.8.2)

#project name and cpp standard (c++11)
project("CMake Test" CXX)
set(CMAKE_CXX_STANDARD 14)
add_compile_options(-g)
# add_compile_options(-g -Weffc++)
set(app_name "demo_sdl_gui")
set(lib_name "sdl_gui")

#include_directories(src)#will search inside all directories inside src
add_subdirectory(src)

#DEMO FILES
#Get include directories from sub folders
set(DEMO_INCLUDE_DIRECTORIES "")
get_directory_property(DEMO_INCLUDE_DIRECTORIES DIRECTORY src DEFINITION DEMO_INCLUDE_DIRECTORIES)
#Add include directories
include_directories(${DEMO_INCLUDE_DIRECTORIES})

#Get Source files paths from subfolders
set(DEMO_SOURCE_FILES "")
get_directory_property(DEMO_SOURCE_FILES DIRECTORY src DEFINITION DEMO_SOURCE_FILES)

#Get Header files paths
set(DEMO_HEADER_FILES "")
get_directory_property(DEMO_HEADER_FILES DIRECTORY src DEFINITION DEMO_HEADER_FILES)


#GUI FILES
#Get include directories from sub folders
set(GUI_INCLUDE_DIRECTORIES "")
get_directory_property(GUI_INCLUDE_DIRECTORIES DIRECTORY src DEFINITION GUI_INCLUDE_DIRECTORIES)
#Add include directories
include_directories(${GUI_INCLUDE_DIRECTORIES})

#Get Source files paths from subfolders
set(GUI_SOURCE_FILES "")
get_directory_property(GUI_SOURCE_FILES DIRECTORY src DEFINITION GUI_SOURCE_FILES)

#Get Header files paths
set(GUI_HEADER_FILES "")
get_directory_property(GUI_HEADER_FILES DIRECTORY src DEFINITION GUI_HEADER_FILES)


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
# find_package(SDL2_mixer REQUIRED)
# include_directories(${SDL2_MIXER_INCLUDE_DIR})
#SDL2 net
#find_package(SDL2_net REQUIRED)
#include_directories(${SDL2_NET_INCLUDE_DIR})

#GUI LIB
add_library(${lib_name} SHARED ${GUI_SOURCE_FILES} ${GUI_HEADER_FILES})

#DEMO EXECUTABLE
add_executable(${app_name} ${DEMO_SOURCE_FILES} ${DEMO_HEADER_FILES})
#change rpath
set_target_properties(${app_name} PROPERTIES LINK_FLAGS "-Wl,-rpath,./")#add local folder as relative path

target_link_libraries(${app_name} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY} ${SDL2_MIXER_LIBRARY} ${lib_name})

#copy contents of root folder "data" to folder "data" next to executable, if build is succeeds(POST_BUILD)
add_custom_command(TARGET ${app_name} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:${app_name}>/data)
