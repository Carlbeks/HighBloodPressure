cmake_minimum_required(VERSION 3.5.0)
project(HighBloodPressure VERSION 0.1.0 LANGUAGES C CXX)

add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)
add_definitions(-DCINTERFACE)
add_definitions(-D__CARLBEKS_CMAKE_VSCODE__)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_WIN32_EXECUTABLE true)
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(${PROJECT_NAME} -Wno-microsoft-string-literal-from-predefined)
endif ()
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/source-charset:utf-8)
    add_compile_options(/execution-charset:utf-8)
endif ()
add_compile_options(/Zc:preprocessor)

include(CTest)
enable_testing()

add_executable(${PROJECT_NAME}
        main.cpp
        def.cpp
        def.h
        InteractManager.cpp
        InteractManager.h
        Renderer.cpp
        Renderer.h
        Game.cpp
        Game.h
        Window.cpp
        Window.h
        Hud.cpp
        Hud.h
        IText.cpp
        IText.h
        exception.cpp
        exception.h
        TextureManager.cpp
        TextureManager.h

        includes.h
        Chars.h
        xWindows.h
        TestCode.h
)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

