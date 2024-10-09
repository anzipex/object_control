cmake_minimum_required(VERSION 3.10)

# Set policy CMP0072 to prefer GLVND
if (POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()

project(ObjectControl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(PNG REQUIRED)

add_executable(ObjectControl
    main.cpp
    Control.cpp
    Object.cpp
    pngutil.cpp
    textures.cpp
)

target_include_directories(ObjectControl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ObjectControl OpenGL::GL GLUT::GLUT PNG::PNG)

set(TEXTURE_FILES
    textures/templateBackground.png
    textures/templateBackgroundSpace.png
    textures/templateObject.png
    textures/templateObjectUfo.png
)

add_custom_command(TARGET ObjectControl POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:ObjectControl>/textures
)

foreach(texture ${TEXTURE_FILES})
    add_custom_command(TARGET ObjectControl POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/${texture}
        $<TARGET_FILE_DIR:ObjectControl>/textures/
    )
endforeach()
