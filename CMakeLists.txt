cmake_minimum_required(VERSION 3.10)

# Set policy CMP0072 to prefer GLVND
if (POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()

project(object_control)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(PNG REQUIRED)

add_executable(object_control
    main.cpp
    control.cpp
    object.cpp
    pngutil.cpp
    textures.cpp
)

target_include_directories(object_control PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(object_control OpenGL::GL GLUT::GLUT PNG::PNG)

set(TEXTURE_FILES
    textures/templateBackground.png
    textures/templateBackgroundSpace.png
    textures/templateObject.png
    textures/templateObjectUfo.png
)

add_custom_command(TARGET object_control POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:object_control>/textures
)

foreach(texture ${TEXTURE_FILES})
    add_custom_command(TARGET object_control POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/${texture}
        $<TARGET_FILE_DIR:object_control>/textures/
    )
endforeach()
