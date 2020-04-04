add_executable(categories_component Categories.cpp Menu.cpp)
target_link_libraries(categories_component PRIVATE ${MENU_LIBRARY} ${PANEL_LIBRARY} ${CURSES_LIBRARIES})
target_include_directories(categories_component PRIVATE ${CPR_INCLUDE_DIRS} ${CURSES_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include)
