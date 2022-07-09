cmake_minimum_required(VERSION 3.10)
project(terminal)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb")
SET(PROJECT_ROOT_PATH "${PROJECT_SOURCE_DIR}")
SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin/")
SET(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib/")
link_directories(${PROJECT_BINARY_DIR}/lib/)

add_compile_options(-fPIC)

include_directories(
    /usr/include/qtermwidget5
	/usr/include/x86_64-linux-gnu/qt5 
	/usr/include/x86_64-linux-gnu/qt5/QtWidgets 
	/usr/include/x86_64-linux-gnu/qt5/QtGui 
	/usr/include/x86_64-linux-gnu/qt5/QtCore 
)

add_executable(
		terminal

		minimal-term.cpp
)
target_link_libraries(
		terminal

		PUBLIC
		-lqtermwidget5 
		-lQt5Widgets 
		-lQt5Gui 
		-lQt5Core 
		-lutf8proc
)