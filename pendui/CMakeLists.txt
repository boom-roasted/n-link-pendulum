cmake_minimum_required(VERSION 3.20)
project(pendui LANGUAGES CXX VERSION 0.0.1)

set(SOURCES
    main.cpp
    Texture.cpp
    MainWindow.cpp
    PendulumProvider.cpp
    Rectangle.cpp
    Menu.cpp OptionsMenu.cpp
    Text.cpp
    Icon.cpp
    Timer.cpp
    FpsCounter.cpp
    Playback.cpp
    Navigator.cpp
    Window.cpp
    Renderer.cpp
    Hotkey.cpp
    Clickable.cpp
    DragHandler.cpp
    Slider.cpp
    Grid.cpp
)

add_executable(pendui ${SOURCES})
target_compile_features(pendui PUBLIC cxx_std_20)
target_link_libraries(pendui PUBLIC SDL2main SDL2 SDL2_image SDL2_ttf PUBLIC pendlib)
# target_include_directories(pendui PUBLIC pendlib)


# Copy resource files to the target build directory
set(RESOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/res/DejaVuSerif.ttf
    ${CMAKE_CURRENT_SOURCE_DIR}/res/dot.png
    ${CMAKE_CURRENT_SOURCE_DIR}/res/framebackward.png
    ${CMAKE_CURRENT_SOURCE_DIR}/res/frameforward.png
    ${CMAKE_CURRENT_SOURCE_DIR}/res/pause.png
    ${CMAKE_CURRENT_SOURCE_DIR}/res/pin.png
    ${CMAKE_CURRENT_SOURCE_DIR}/res/play.png
)
add_custom_command(
    TARGET pendui POST_BUILD
    COMMAND
        mkdir -p $<TARGET_FILE_DIR:pendui>/res &&
        ${CMAKE_COMMAND} -E copy_if_different
        ${RESOURCES}
        $<TARGET_FILE_DIR:pendui>/res
    )

# install(TARGETS pendui RUNTIME DESTINATION bin)