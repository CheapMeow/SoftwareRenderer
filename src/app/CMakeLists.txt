file(GLOB_RECURSE APP_HEADER_FILES CONFIGURE_DEPENDS ./*.h ./*.hpp)
file(GLOB_RECURSE APP_SOURCE_FILES CONFIGURE_DEPENDS ./*.cpp)

add_executable(${APP_NAME} ${APP_HEADER_FILES} ${APP_SOURCE_FILES})

set_target_properties(${APP_NAME} PROPERTIES CXX_STANDARD 20)

target_compile_definitions(${APP_NAME} PUBLIC _USE_MATH_DEFINES)

target_include_directories(${APP_NAME} PUBLIC ./)

target_link_libraries(${APP_NAME} PUBLIC SDL2main SDL2::SDL2)
target_include_directories(${APP_NAME} PUBLIC ${3RDPARTY_ROOT_DIR}/SDL/include)

target_include_directories(${APP_NAME} PUBLIC ${3RDPARTY_ROOT_DIR}/stb)

add_custom_command(
    TARGET ${APP_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${APP_NAME}> $<TARGET_FILE_DIR:${APP_NAME}> COMMAND_EXPAND_LISTS)