cmake_minimum_required(VERSION 3.10)
project(xsh)

set(CMAKE_CXX_STANDARD 14)

add_compile_options(-fPIC)


add_executable(
		xsh

		shell.cpp
)

target_link_libraries(
		xsh

		PUBLIC
		-lreadline

)