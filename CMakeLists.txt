cmake_minimum_required(VERSION 3.10)

project(xsh)

set(CMAKE_CXX_STANDARD 14)
SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin/")

add_compile_options(-fPIC)


add_executable(
		xsh

		shell.cpp
		tools.cpp
		
)

target_link_libraries(
		xsh

		PUBLIC
		-lreadline

)

project(terminal)
add_compile_options(-fPIC)
SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin/")

include_directories(
    /usr/include/qtermwidget5
	/usr/include/x86_64-linux-gnu/qt5 
	/usr/include/x86_64-linux-gnu/qt5/QtWidgets 
	/usr/include/x86_64-linux-gnu/qt5/QtGui 
	/usr/include/x86_64-linux-gnu/qt5/QtCore 
)

add_executable(
		terminal

		terminalInterface/minimal-term.cpp
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