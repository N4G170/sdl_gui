###########################
#         sdl_gui         #
###########################

set(DEMO_INCLUDE_DIRECTORIES src/demo)

set(DEMO_SOURCE_FILES "")
set(FILES_PATH "src/demo/")#file path from root
list(APPEND DEMO_SOURCE_FILES "${FILES_PATH}sdl_gui_demo.cpp")
list(APPEND DEMO_SOURCE_FILES "${FILES_PATH}sdl_init.cpp")
list(APPEND DEMO_SOURCE_FILES "${FILES_PATH}main.cpp")
list(APPEND DEMO_SOURCE_FILES "${FILES_PATH}animation_panel.cpp")
list(APPEND DEMO_SOURCE_FILES "${FILES_PATH}buttons_panel.cpp")
list(APPEND DEMO_SOURCE_FILES "${FILES_PATH}bars_panel.cpp")
list(APPEND DEMO_SOURCE_FILES "${FILES_PATH}others_panel.cpp")

set(DEMO_HEADER_FILES "")
set(FILES_PATH "src/demo/")#file path from root
list(APPEND DEMO_HEADER_FILES "${FILES_PATH}sdl_gui_demo.hpp")
list(APPEND DEMO_HEADER_FILES "${FILES_PATH}sdl_init.hpp")
list(APPEND DEMO_HEADER_FILES "${FILES_PATH}constants.hpp")
list(APPEND DEMO_HEADER_FILES "${FILES_PATH}animation_panel.hpp")
list(APPEND DEMO_HEADER_FILES "${FILES_PATH}buttons_panel.hpp")
list(APPEND DEMO_HEADER_FILES "${FILES_PATH}bars_panel.hpp")
list(APPEND DEMO_HEADER_FILES "${FILES_PATH}others_panel.hpp")
