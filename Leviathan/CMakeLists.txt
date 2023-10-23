# CMakeList.txt : CMake project for Leviathan, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Leviathan "Leviathan.cpp"  "UI.cpp"  "UI.hpp" "Connection.cpp" "Connection.hpp" "ConnDetails.hpp" "LoadTexture.hpp" "Management.hpp" "stb_image.hpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Leviathan PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed
find_package(glad CONFIG REQUIRED)
target_link_libraries(Leviathan PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(Leviathan PRIVATE glfw)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(Leviathan PRIVATE imgui::imgui)

find_package(asio CONFIG REQUIRED)
target_link_libraries(Leviathan PRIVATE asio::asio)