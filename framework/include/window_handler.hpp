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
  void set_callback_object(GLFWwindow* window, Application* app);
  // free resources
  void close_and_quit(GLFWwindow* window, int status);
    // calculate fps and show in window title
  void show_fps(GLFWwindow* window);
}

#endif