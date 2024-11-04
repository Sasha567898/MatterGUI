# CMakeLists.txt

# Specify the minimum version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(hello)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(
    /usr/local/lib/wx/include/gtk3-unicode-3.3
    /usr/local/include/wx-3.3	
	 /usr/local/include 
)

# Source files
set(SOURCES
    MainFrame.cpp
    App.cpp
    Server.cpp
)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FILE_OFFSET_BITS=64 -DWXUSINGDLL -D__WXGTK__")

# Linker flags
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

# Define the executable
add_executable(hello ${SOURCES})

# Link the wxWidgets libraries
target_link_libraries(hello
    wx_gtk3u_xrc-3.3
    wx_gtk3u_html-3.3
    wx_gtk3u_qa-3.3
    wx_gtk3u_adv-3.3
    wx_gtk3u_core-3.3
    wx_baseu_xml-3.3
    wx_baseu_net-3.3
    wx_baseu-3.3
	 wiringPi
)
file(COPY resources/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
