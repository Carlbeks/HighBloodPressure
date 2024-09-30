cmake_minimum_required(VERSION 3.5.0)
project(HighBloodPressure VERSION 0.1.0 LANGUAGES C CXX)

#add_definitions(-DUNICODE)
#add_definitions(-D_UNICODE)
#add_definitions(-DCINTERFACE)
#add_definitions(-D__CARLBEKS_CMAKE_VSCODE__)
add_compile_definitions(DUNICODE)
add_compile_definitions(_UNICODE)
add_compile_definitions(CINTERFACE)
add_compile_definitions(__CARLBEKS_CMAKE_VSCODE__)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_WIN32_EXECUTABLE true)

include(CTest)
enable_testing()

add_executable(HighBloodPressure main.cpp)
include(CPack)
