#ifndef LAUNCHER_HPP
#define LAUNCHER_HPP

#include "application.hpp"

#include <string>

// forward declarations
class Application;
class GLFWwindow;

namespace window_handler { 
// create window and set callbacks
GLFWwindow* initialize(unsigned width, unsigned height);
// load shader programs and update uniform locations
void set_callback_object(Application* app, GLFWwindow* window);
// free resources
void close_and_quit(GLFWwindow* window, int status);
  // calculate fps and show in window title
void show_fps(GLFWwindow* window);
}

void update_shader_programs(std::map<std::string, shader_program>& shaders, bool throwing);
std::string read_resource_path(int argc, char* argv[]);
glm::fmat4 calculate_projection_matrix(unsigned width, unsigned height);

#endif