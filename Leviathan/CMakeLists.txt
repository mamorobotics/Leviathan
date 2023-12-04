# CMakeList.txt : CMake project for Leviathan, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Leviathan "Leviathan.cpp"  "UI.cpp"  "UI.hpp" "Connection.cpp" "Connection.hpp" "ControllerValues.hpp" "Controller.cpp" "Controller.hpp" "ConnDetails.hpp" "LoadTexture.hpp" "Management.hpp" "stb_image.hpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Leviathan PROPERTY CXX_STANDARD 20)
endif()

set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")
message( ${LIB_DIR} )

# Glad
set(GLAD_DIR "${LIB_DIR}/glad")

add_library("glad" "${GLAD_DIR}/src/glad.c")

target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")

# ImGui
set(IMGUI_DIR "${LIB_DIR}/ImGui")

add_library("ImGui" "${IMGUI_DIR}/imgui.cpp"
"${IMGUI_DIR}/imgui_demo.cpp"
"${IMGUI_DIR}/imgui_draw.cpp"
"${IMGUI_DIR}/imgui_tables.cpp"
"${IMGUI_DIR}/imgui_widgets.cpp"

"${IMGUI_DIR}/imgui_impl_glfw.cpp"
"${IMGUI_DIR}/imgui_impl_opengl3.cpp")

target_include_directories("ImGui" PRIVATE "${IMGUI_DIR}")
target_include_directories(${PROJECT_NAME} PRIVATE "${IMGUI_DIR}")
target_link_libraries(${PROJECT_NAME} "ImGui" "${CMAKE_DL_LIBS}")

# Glfw3
set(GLFW_DIR "${LIB_DIR}/glfw3")

set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")

add_subdirectory("${GLFW_DIR}")

target_link_libraries(${PROJECT_NAME} "glfw" "${GLFW_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLFW_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")

# asio

include_directories("${LIB_DIR}/asio/include")