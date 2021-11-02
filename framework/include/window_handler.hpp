#ifndef WINDOW_HANDLER_HPP
#define WINDOW_HANDLER_HPP

#include <glm/gtc/type_precision.hpp>

// dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// forward declarations
class Application;
struct GLFWwindow;

namespace window_handler {
// create window and set callbacks
GLFWwindow* initialize(glm::uvec2 const& resolution,
                       unsigned ver_major,
                       unsigned ver_minor);
// load shader programs and update uniform locations
void set_callback_object(GLFWwindow* window, Application* app);
// free resources
void close_and_quit(GLFWwindow* window, int status);
// calculate fps and show in window title
void show_fps(GLFWwindow* window);
}  // namespace window_handler

#endif