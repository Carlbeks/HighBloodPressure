cmake_minimum_required(VERSION 3.5.0)
project(HighBloodPressure VERSION 0.1.0 LANGUAGES C CXX)

add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)
add_definitions(-DCINTERFACE)
add_definitions(-D__CARLBEKS_CMAKE_VSCODE__)
set(CMAKE_CXX_STANDARD 26)
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
        src/main.cpp
        src/def.h
        src/def.cpp
        src/interact/InteractManager.h
        src/interact/InteractManager.cpp
        src/render/Renderer.h
        src/render/Renderer.cpp
        src/game/Game.h
        src/game/Game.cpp
        src/ui/Window.h
        src/ui/Window.cpp
        src/ui/Hud.h
        src/ui/Hud.cpp
        src/utils/IText.h
        src/utils/IText.cpp
        src/utils/exception.h
        src/utils/exception.cpp
        src/utils/File.h
        src/utils/File.cpp
        src/render/TextureManager.h
        src/render/TextureManager.cpp

        src/includes.h
        src/utils/Chars.h
        src/ui/xWindows.h
        src/utils/TestCode.h
        src/game/entity/Entity.h
        src/game/entity/Player.h
        src/game/entity/Player.cpp
        src/game/entity/Damage.h
        src/utils/math.h
        src/game/world/World.h
        src/game/world/Location.h
        src/game/world/Block.h
)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../exenv/")
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/../exenv/")
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/../exenv/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../exenv/")

include(CPack)

